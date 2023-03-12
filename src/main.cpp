#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "gears.h"
#include "bsp/board.h"
#include "tusb.h"
#include "tusb_config.h"

#define GPIO_SWITCH 15

using namespace std;

Gear gear;

extern void hid_app_task(void);
void led_blinking_task(void);
void report_status(void);
void led_init(void);

void gpio_callback(uint gpio, uint32_t events);
void gpio_handbreak(uint gpio, uint32_t events);

extern "C" int main()
{
    // Enable UART so we can print status output
    stdio_init_all();

    board_init();
    printf("\n<Reboot>\n");
    led_init();

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c/bus_scan example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else

    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    i2c_init(i2c_default, 100 * 1000);

    gpio_set_irq_enabled_with_callback(GPIO_SWITCH, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    tusb_init();

    gpio_callback(GPIO_SWITCH, 0);

    while (true)
    {
        // tinyusb host task
        tuh_task();

        led_blinking_task();
        report_status();

#if CFG_TUH_HID
        hid_app_task();
#endif
    }

    return 0;

#endif
}

void led_init(void)
{
    bool led_state = false;

    for (uint8_t i = 0; i < 11; i++)
    {
        board_led_write(led_state);
        led_state = 1 - led_state; // toggle
        sleep_ms(70);
    }
}

void led_blinking_task(void)
{
    const uint32_t interval_ms = 1000;
    static uint32_t start_ms = 0;

    static bool led_state = false;

    // Blink every interval ms
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

void report_status(void)
{
    const uint32_t interval_ms = 1000;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    printf("Mode: %c\r\n", gear.get_mode());
    switch (gear.get_mode())
    {
    case 'H':
        printf("Gear: %d\r\n", gear.get_gear_h());
        break;

    case 'S':
        printf("Position: %c\r\n", gear.get_gear_sequencial());
    }
}

void gpio_callback(uint gpio, uint32_t events)
{

    if (gpio_get(gpio))
        gear.set_gear_mode(GearMode::S);
    else
        gear.set_gear_mode(GearMode::H);

    printf("Gear Mode: %c\r\n", gear.get_mode());
}
