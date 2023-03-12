#pragma once

#include <iostream>
#include "hardware/i2c.h"

enum GearSequencial : uint8_t
{
    center = 0x04,
    down,
    up
};

enum GearMode : uint8_t
{
    S = 0x00,
    H = 0x80
};

class Gear
{

private:
    uint8_t m_size = 14;
    uint8_t m_addr = 0x01;
    uint8_t m_data[14] = {
        0x00, // Shifter mode 0 - S / 0x80 - H
        0x0C, // Unknown
        0x01, // Unknown
        0x00, // Gear in H-mode
        0x04, // Gear in S-Mode 0x04 - center, 0x05 - down, 0x06 - up
        0x80, // Unknown
        0x80, // Unknown
        0x00, // Y cordinate
        0x00, // X cordinate
        0x00, // Unknown
        0x00, // Unknown
        0x00, // Unknown
        0x00, // Unknown
        0x00  // Unknown
    };

public:
    void set_gear_mode(GearMode mode);
    void set_gear_sequencial(GearSequencial gear);
    void set_gear_h(uint8_t gear, bool direct);
    unsigned char get_mode();
    uint8_t get_gear_h();
    unsigned char get_gear_sequencial();
    uint8_t *get_data();
    uint8_t get_size();
    void send(i2c_inst_t *i2c);
    void send(i2c_inst_t *i2c, uint8_t addr);
};