#pragma once

#include <iostream>
#include "hardware/i2c.h"

class Handbrake
{
private:
    uint8_t m_size = 14;
    uint8_t m_addr = 0x01;

    uint8_t m_data[14] = {
        0x02, // TSS handbrake
        0x0C, // Unknown
        0x02, // Unknown
        0x00, // Unknown, 01 when max break
        0xFF, // FF - no break, 00 max break
        0x80, // Unknown ◄─┐
        0x81, // Unknown ◄─│─┐
        0x80, // Unknown ◄─┘ │
        0x81, // Unknown ◄───┘
        0x00, // Unknown
        0x00, // Unknown
        0x00, // Unknown
        0x00, // Unknown
        0x00  // Unknown
    };

    uint8_t m_min[4] = {
        0x80, // Unknown ◄─┐
        0x81, // Unknown ◄─│─┐
        0x80, // Unknown ◄─┘ │
        0x81, // Unknown ◄───┘
    };

    uint8_t m_max[4] = {
        0x7F, // Unknown ◄─┐
        0x1D, // Unknown ◄─│─┐
        0x7F, // Unknown ◄─┘ │
        0x1D, // Unknown ◄───┘
    };

public:
    void set_handbrake(uint8_t value);
    void send(i2c_inst_t *i2c);
    uint8_t *get_data();
    void send(i2c_inst_t *i2c, uint8_t addr);
};

/**
 * TSS Sparco handbrake I2C data
 *
 * Handbrake min -> max
 *
 * 0  02 02 02 02 02 // TSS handbrake
 * 1  0C 0C 0C 0C 0C // Unknown
 * 2  02 02 02 02 02 // Unknown
 * 3  00 00 00 01 01 // Unknown, 01 when max break
 * 4  FF 8A 82 17 0F // FF - no break, 00 max break
 * 5  80 80 80 7F 7F // Unknown ◄─┐
 * 6  81 4D 4A 20 1D // Unknown ◄─│─┐
 * 7  80 80 80 7F 7F // Unknown ◄─┘ │
 * 8  81 4D 4A 20 1D // Unknown ◄───┘
 * 9  00 00 00 00 00 // Unknown
 * A  00 00 00 00 00 // Unknown
 * B  00 00 00 00 00 // Unknown
 * C  00 00 00 00 00 // Unknown
 * D  00 00 00 00 00 // Unknown
 *
 * https://www.isrtv.com/forums/topic/24532-gearbox-connector-on-base/?do=findComment&comment=232234
 */