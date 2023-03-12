#include "handbrake.h"

void path_data(uint8_t (&data)[14], uint8_t (&update)[4])
{
    for (uint8_t i = 0; i < 4; i++)
    {
        data[i + 5] = update[i];
    }
}

void Handbrake::set_handbrake(uint8_t value)
{
    this->m_data[3] = 0x0;

    path_data(this->m_data, this->m_min);

    this->m_data[4] = value;

    // Uncertain assumption that these values needs to be
    // changed when the value is lowered than 0x20.
    if (value < 0x20)
    {
        this->m_data[3] = 0x01;
        path_data(this->m_data, this->m_max);
    }
}

uint8_t *Handbrake::get_data()
{
    return static_cast<uint8_t *>(this->m_data);
}

void Handbrake::send(i2c_inst_t *i2c)
{
    this->send(i2c, this->m_addr);
}

void Handbrake::send(i2c_inst_t *i2c, uint8_t addr)
{
    auto data = this->get_data();

    // for (int i = 0; i < 14; i++)
    // {
    //     printf("%02X ", m_data[i]);
    // }

    i2c_write_blocking(i2c, addr, data, this->m_size, false);
}
