/*
    Simple SOS blink example in C++

*/
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "gears.h"
#include "bsp/board.h"
#include "tusb.h"
#include "tusb_config.h"

using namespace std;

static uint8_t g_addr = 0x01;

extern void hid_app_task(void);
void led_blinking_task(void);

void send(i2c_inst_t *i2c, Gear &gear)
{
    auto data = gear.get_data();

    for (int i = 0; i < 14; i++)
    {
        printf("%02X ", data[i]);
    }

    i2c_write_blocking(i2c, g_addr, data, gear.get_size(), true);
}

int main()
{
    // Enable UART so we can print status output
    stdio_init_all();

    board_init();

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c/bus_scan example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else

    // This example will use I2C0 on the default SDA and SCL pins(GP4, GP5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    printf("\n<Reboot>\n");

    tusb_init();

    while (true)
    {
        // tinyusb host task
        tuh_task();

        led_blinking_task();

#if CFG_TUH_HID
        hid_app_task();
#endif
    }

    return 0;

#endif
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