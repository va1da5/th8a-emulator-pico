#include "gears.h"

void Gear::set_gear_mode(GearMode mode)
{
    this->m_data[0] = mode;
}

void Gear::set_gear_sequencial(GearSequencial gear)
{
    this->m_data[4] = gear;
}

void Gear::set_gear_h(uint8_t gear)
{
    this->m_data[3] = (0x80 >> (8 - gear));
}

unsigned char Gear::get_mode()
{
    if (this->m_data[0] == GearMode::H)
    {
        return 'H';
    }
    return 'S';
}

uint8_t Gear::get_gear_h()
{
    uint8_t value = this->m_data[3];

    for (uint8_t i = 0; i < 9; i++)
    {
        if (value == (0x80 >> (8 - i)))
        {
            return i;
        }
    }
    return 0;
}

unsigned char Gear::get_gear_sequencial()
{
    switch (this->m_data[4])
    {
    case GearSequencial::center:
        return 'C';
    case GearSequencial::down:
        return 'D';
    case GearSequencial::up:
        return 'U';
    default:
        return 'C';
    }
}

uint8_t *Gear::get_data()
{
    return static_cast<uint8_t *>(this->m_data);
}

uint8_t Gear::get_size()
{
    return this->m_size;
}

void Gear::send(i2c_inst_t *i2c, uint8_t addr)
{
    auto data = this->get_data();

    for (int i = 0; i < 14; i++)
    {
        printf("%02X ", m_data[i]);
    }

    i2c_write_blocking(i2c, addr, data, m_size, true);
}