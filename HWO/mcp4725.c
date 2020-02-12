/*
 * mcp4725.c
 *
 * Created: 05-Jun-19 17:57:40
 * Changed: 7.11.2019
 *  Author: Edwin
 */ 

#include "mcp4725.h"
#include <stdint.h>

// TODO: find out needed #defines
#define MCP4725_A0_BIT 0b00000010
#define MCP4725_ADDRESS_MASK 0b00000010
#define MCP4725_ADDRESS_BASE 0b11000100

void mcp4725_init(mcp4725_t* device,
                  u8_fptr_u8_pu8_u8_t i2c_tx_cb,
                  mcp4725_addr_t address,
                  mcp4725_pwrd_md_t power_down_mode,
                  uint16_t dac_data,
                  uint16_t eemprom_data)
{
    uint8_t temp[6];

    // update device attributes
    device->i2c_tx = i2c_tx_cb;
    device->address = address;
    device->power_down_mode = power_down_mode;
    device->dac_data = dac_data;
    device->eemprom_data = eemprom_data;

    // setting up data set to be sent to the device
    temp[0] = mcp4725_cmd_WRITE_DAC_AND_EEPROM | (power_down_mode << 1);
    temp[1] = (uint8_t)(dac_data >> 4);
    temp[2] = (uint8_t)(dac_data << 4);
    temp[3] = temp[0];
    temp[4] = temp[1];
    temp[5] = temp[2];

    // writing to the device
    device->i2c_tx((uint8_t)(device->address), temp, 6);
}

void mcp4725_write_DAC(const mcp4725_t* device, uint16_t value)
{
    uint8_t temp[3];

    // setting up data set
    temp[0] = mcp4725_cmd_WRITE_DAC | (device->power_down_mode << 1);
    temp[1] = (uint8_t)(value >> 4);
    temp[2] = (uint8_t)(value << 4);

    device->i2c_tx(device->address, temp, 3);

}

void mcp4725_write_DAC_and_EEPROM(const mcp4725_t* device, uint16_t value)
{
    uint16_t temp = (value & 0x0FFFF) << 4;
    uint8_t data[3] = {0x60 | (device->power_down_mode >> 3),
                       (uint8_t)(value >> 8),
                       (uint8_t)(value)
    };

    device->i2c_tx(MCP4725_ADDRESS_BASE | (device->address), data, 3);
}

void mcp4725_set_powerdown_impedance(mcp4725_t* device, mcp4725_pdwn_imp_t impedance)
{
    device->power_down_mode = impedance;
}


#undef MCP4725_ADDRESS_BASE
/*

#ifdef old
//#include "dummy.h"

callback_1_t cb_i2c_write = 0;
callback_1_t cb_i2c_start = 0;
callback_t cb_i2c_stop = 0;
u8_fptr_u8_pu8_u8 i2c_transmit = 0;


uint8_t power_down_impedance = NORMAL_MODE;
#ifdef out
void mcp4725_register_i2c_start(callback_1_t start)
{
	cb_i2c_start = start;
}
void mcp4725_regsiter_i2c_write(callback_1_t write)
{
	cb_i2c_write = write;
}
void mcp4725_register_i2c_stop(callback_t stop)
{
	cb_i2c_stop = stop;
}

#endif // out

void mcp4725_init(u8_fptr_u8_pu8_u8 i2c_tx)
{
	i2c_transmit = i2c_tx;
}

void mcp4725_write_DAC(uint8_t address, uint16_t data)
{
	uint8_t data[3] = {
						(0x40 | power_down_impedance),
						(uint8_t)(data >> 4),
						(uint8_t)(data << 4)
	};
	address = (address & MCP4725_ADDRESS_MASK) | MCP4725_ADDRESS_BASE;
	i2c_transmit(address, data, 3);
	
	#ifdef out
	data &= 0x0FFF;
	data = data << 4;
	if(cb_i2c_start(address)) {
		cb_i2c_stop();
		return;
	}
	cb_i2c_write(0x40 | power_down_impedance); // write to dac register
	cb_i2c_write((uint8_t)(data >> 8));
	cb_i2c_write((uint8_t)(data));
	
	cb_i2c_stop();
	#endif // out
}

void mcp4725_eeprom_write(uint8_t address, uint16_t data)
{
	uint8_t data[3] = {
						(0x60 | power_down_impedance),
						(uint8_t)(data >> 4),
						(uint8_t)(data << 4)
	};
	address = (address & MCP4725_ADDRESS_MASK) | MCP4725_ADDRESS_BASE;
	i2c_transmit(address, data, 3);
	
	#if 0
	data &= 0x0FFF;
	data = data << 4;
	if(cb_i2c_start(address)) {
		cb_i2c_stop();
		return;
	}
	cb_i2c_write(0x60 | power_down_impedance); // write to dac and eeprom register
	cb_i2c_write((uint8_t)(data >> 8));
	cb_i2c_write((uint8_t)(data));
	
	cb_i2c_stop(); 
#endif
}

void mcp4725_set_powerdown_impedance(mcp4725_pdwn_imp_t imp)
{
	power_down_impedance = imp & 0x30;
}

#endif

*/
