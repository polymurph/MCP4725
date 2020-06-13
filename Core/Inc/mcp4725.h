#ifndef _MCP4725_H_
#define _MCP4725_H_

#include <functional>
#include <stdint.h>

using namespace std;

class MCP4725
{
    public:

    typedef function<void(uint8_t, uint8_t*, uint8_t)>i2c_write_t;

    //typedef void(*i2c_write_t)(uint8_t, uint8_t*, uint8_t);

    typedef enum {
        i2c_addr_0x0 = 0b01100000,
        i2c_addr_0x1 = 0b01100001,
        i2c_addr_0x2 = 0b01100010,
        i2c_addr_0x3 = 0b01100011,
        i2c_addr_0x4 = 0b01100100,
        i2c_addr_0x5 = 0b01100101,
        i2c_addr_0x6 = 0b01100110,
        i2c_addr_0x7 = 0b01100111
    }i2c_addr_t;

    typedef enum{
        pwrd_md_NORMAL = 0x00,
        pwrd_md_1k_ohm = 0x01,
        pwrd_md_100k_ohm = 0x02,
        pwrd_md_500k_ohm = 0x03
    }pwrd_md_t;
    
    MCP4725(i2c_write_t i2c_write,
            i2c_addr_t address,
            pwrd_md_t power_down_mode,
            uint16_t dac_value,
            uint16_t eemprom_value);

    void set_DAC(uint16_t value);

    void set_DAC_and_EEPROM(uint16_t value);

    // TODO: complete implementation
    void set_powerdown_impedance(pwrd_md_t power_down_mode);

    private:
    
    i2c_write_t i2c_write;
    i2c_addr_t address;
    pwrd_md_t power_down_mode;
    uint16_t dac_value;
    uint16_t eeprom_value;

    typedef enum {
        mcp4725_cmd_FAST_MODE = 0x00,
        mcp4725_cmd_WRITE_DAC = 0x40,
        mcp4725_cmd_WRITE_DAC_AND_EEPROM = 0x60
    }mcp4725_cmd_t;
};

#endif // _MCP4725_H_
