/*
 * mcp4725.c
 *
 * Created: 05-Jun-19 17:57:40
 * Changed: 7.11.2019
 *  Author: Edwin
 */ 

#include "mcp4725.h"
#include <stdint.h>
//#include "dummy.h"

callback_1_t cb_i2c_write = 0;
callback_1_t cb_i2c_start = 0;
callback_t cb_i2c_stop = 0;
u8_fptr_u8_pu8_u8 i2c_transmit = 0;


uint8_t power_down_impedance = NORMAL_MODE;
#if 0
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
#endif

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
	
	#if 0
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
	#ednif
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