#include "mcp4725.h"



MCP4725::MCP4725(i2c_write_t i2c_write,
                 i2c_addr_t address,
                 pwrd_md_t power_down_mode,
                 uint16_t dac_value,
                 uint16_t eemprom_value) :
                 i2c_write(i2c_write),
                 address(address),
                 power_down_md(power_down_mode),
                 dac_value(dac_value),
                 eeprom_value(eeprom_value)
{
    uint8_t temp[5];

    temp[0] = mcp4725_cmd_WRITE_DAC_AND_EEPROM | (power_down_mode << 1);
    temp[1] = (uint8_t)(dac_value >> 4);
    temp[2] = (uint8_t)(dac_value << 4);
    temp[3] = temp[0];
    temp[4] = temp[1];
    temp[5] = temp[2];

    // writing to device
    i2c_write(address, temp, 6);
}

void MCP4725::set_ADC(uint8_t value)
{
    uint8_t temp[3];

    dac_value = value;

    // setting up data set
    temp[0] = mcp4725_cmd_WRITE_DAC | (power_down_mode << 1);
    temp[1] = (uint8_t)(value >> 4);
    temp[2] = (uint8_t)(value << 4);

    i2c_write(address, temp, 3);
}

void MCP4725::set_DAC_and_EEPROM(uint16_t value)
{
    uint8_t temp[6];

    dac_value = value;
    eemprom_value = value;

    // setting up data set to be sent to the device
    temp[0] = mcp4725_cmd_WRITE_DAC_AND_EEPROM | (power_down_mode << 1);
    temp[1] = (uint8_t)(value >> 4);
    temp[2] = (uint8_t)(value << 4);
    temp[3] = temp[0];
    temp[4] = temp[1];
    temp[5] = temp[2];

    // writing to the device
    i2c_write(address, temp, 6);
}

void MCP4725::set_powerdown_impedance(pwrd_md_t power_down_md)
{

}
