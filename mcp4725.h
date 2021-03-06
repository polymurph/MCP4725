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

#ifndef MCP4725_H_
#define MCP4725_H_
#include <stdint.h>

typedef uint8_t (*u8_fptr_u8_pu8_u8_t)(uint8_t,const uint8_t*, uint8_t);

typedef enum {
    mcp4725_addr_0x0 = 0b01100000,
    mcp4725_addr_0x1 = 0b01100001,
    mcp4725_addr_0x2 = 0b01100010,
    mcp4725_addr_0x3 = 0b01100011,
    mcp4725_addr_0x4 = 0b01100100,
    mcp4725_addr_0x5 = 0b01100101,
    mcp4725_addr_0x6 = 0b01100110,
    mcp4725_addr_0x7 = 0b01100111
}mcp4725_addr_t;

typedef enum {
    mcp4725_cmd_FAST_MODE = 0x00,
    mcp4725_cmd_WRITE_DAC = 0x40,
    mcp4725_cmd_WRITE_DAC_AND_EEPROM = 0x60
}mcp4725_cmd_t;

typedef enum{
    mcp4725_pwrd_md_NORMAL = 0x00,
    mcp4725_pwrd_md_1k_ohm = 0x01,
    mcp4725_pwrd_md_100k_ohm = 0x02,
    mcp4725_pwrd_md_500k_ohm = 0x03
}mcp4725_pwrd_md_t;

// mcp4725 class
typedef struct{
    u8_fptr_u8_pu8_u8_t i2c_tx;
    mcp4725_addr_t address;
    mcp4725_pwrd_md_t power_down_mode;
    uint16_t dac_value;
    uint16_t eemprom_value;
}mcp4725_t;

void mcp4725_init(mcp4725_t* device,
                  u8_fptr_u8_pu8_u8_t i2c_tx_cb,
                  mcp4725_addr_t address,
                  mcp4725_pwrd_md_t power_down_mode,
                  uint16_t dac_value,
                  uint16_t eemprom_value);

void mcp4725_write_DAC(mcp4725_t* device, uint16_t value);

void mcp4725_write_DAC_and_EEPROM(mcp4725_t* device, uint16_t value);

void mcp4725_set_powerdown_impedance(mcp4725_t* device, mcp4725_pwrd_md_t impedance);

#endif /* MCP4725_H_ */