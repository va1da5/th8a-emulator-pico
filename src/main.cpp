/*
    Simple SOS blink example in C++

*/
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "gears.h"

using namespace std;

static int g_addr = 0x01;

Gear gear;

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
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c/bus_scan example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else

    // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    printf("\n<Reboot>\n");

    while (true)
    {

        gear.set_gear_mode(GearMode::S);
        gear.set_gear_sequencial(GearSequencial::center);
        printf("\nS -> Center\n");
        send(i2c_default, gear);

        sleep_ms(500);

        gear.set_gear_sequencial(GearSequencial::up);
        printf("\nS -> Up\n");
        send(i2c_default, gear);

        sleep_ms(500);

        gear.set_gear_sequencial(GearSequencial::down);
        printf("\nS -> Down\n");
        send(i2c_default, gear);

        sleep_ms(500);
        gear.set_gear_mode(GearMode::H);

        for (uint8_t i = 0; i < 9; i++)
        {
            gear.set_gear_h(i);
            printf("\nH -> %i\n", i);
            send(i2c_default, gear);
            sleep_ms(500);
        }

        printf("\n\n\n");
    }

#endif
}