/*
MIT License

Copyright (c) 2019 Edwin Koch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "mcp4725.h"
#include <stdint.h>

void mcp4725_init(mcp4725_t* device,
                  u8_fptr_u8_pu8_u8_t i2c_tx_cb,
                  mcp4725_addr_t address,
                  mcp4725_pwrd_md_t power_down_mode,
                  uint16_t dac_value,
                  uint16_t eemprom_value)
{
    uint8_t temp[6];

    // update device attributes
    device->i2c_tx = i2c_tx_cb;
    device->address = address;
    device->power_down_mode = power_down_mode;
    device->dac_value = dac_value;
    device->eemprom_value = eemprom_value;

    // setting up data set to be sent to the device
    temp[0] = mcp4725_cmd_WRITE_DAC_AND_EEPROM | (power_down_mode << 1);
    temp[1] = (uint8_t)(dac_value >> 4);
    temp[2] = (uint8_t)(dac_value << 4);
    temp[3] = temp[0];
    temp[4] = temp[1];
    temp[5] = temp[2];

    // writing to device
    device->i2c_tx((uint8_t)(device->address), temp, 6);
}

void mcp4725_write_DAC(mcp4725_t* device, uint16_t value)
{
    uint8_t temp[3];

    device->dac_value = value;

    // setting up data set
    temp[0] = mcp4725_cmd_WRITE_DAC | (device->power_down_mode << 1);
    temp[1] = (uint8_t)(value >> 4);
    temp[2] = (uint8_t)(value << 4);

    device->i2c_tx(device->address, temp, 3);
}

void mcp4725_write_DAC_and_EEPROM(mcp4725_t* device, uint16_t value)
{
    uint8_t temp[6];

    device->dac_value = value;
    device->eemprom_value = value;

    // setting up data set to be sent to the device
    temp[0] = mcp4725_cmd_WRITE_DAC_AND_EEPROM | (device->power_down_mode << 1);
    temp[1] = (uint8_t)(value >> 4);
    temp[2] = (uint8_t)(value << 4);
    temp[3] = temp[0];
    temp[4] = temp[1];
    temp[5] = temp[2];

    // writing to the device
    device->i2c_tx((uint8_t)(device->address), temp, 6);
}

void mcp4725_set_powerdown_impedance(mcp4725_t* device, mcp4725_pwrd_md_t impedance)
{
    device->power_down_mode = impedance;
}
