/*
 * mcp4725.h
 *
 * Created: 05-Jun-19 17:57:51
 * Changed: 7.11.2019
 *  Author: Edwin
 */ 


#ifndef MCP4725_H_
#define MCP4725_H_
#include <stdint.h>

#define MCP4725_A0_BIT 0b00000010
#define MCP4725_ADDRESS_MASK 0b00000010 
#define MCP4725_ADDRESS_BASE 0b11000100
//#define MCP4725_ADDRESS (MCP4725_ADDRESS_BASE | MCP4725_A0_BIT)

#if 0
typedef void (*callback_t)();
typedef uint8_t (*callback_1_t)(uint8_t);
#endif

/**
* function pointer prototype with 
*
*/
typedef uint8_t (*u8_fptr_u8_pu8_u8)(uint8_t, *uint8_t, uint8_t);

enum{
	MCP4725_DAC_REG_BIT_PD0 = 0x10,
	MCP4725_DAC_REG_BIT_PD1	= 0x20,
	MCP4725_DAC_REG_BIT_C0 = 0x20,
	MCP4725_DAC_REG_BIT_C1 = 0x40,
	MCP4725_DAC_REG_BIT_C2 = 0x80,
};

typedef enum{
	NORMAL_MODE = 0,
	ZA_1k_ohm = MCP4725_PD0,
	ZA_100k_ohm = MCP4725_PD1,
	ZA_500k_ohm = MCP4725_PD1 | MCP4725_PD0
}mcp4725_pdwn_imp_t;


typedef struct{
    uint8_t address,
    mcp4725_pdwn_imp_t power_down_mode,
    u8_fptr_u8_pu8_u8 i2c_tx,
}mcp74725_t;

#if 0
void mcp4725_register_i2c_start(callback_1_t start);
void mcp4725_regsiter_i2c_write(callback_1_t write);
void mcp4725_register_i2c_stop(callback_t stop);
#endif

void mcp4725_init(u8_fptr_u8_pu8_u8 i2c_tx);

void mcp4725_write_DAC(uint8_t address, uint16_t data);

void mcp4725_eeprom_write(uint8_t address, uint16_t data);

void mcp4725_set_powerdown_impedance(mcp4725_pdwn_imp_t imp);


#endif /* MCP4725_H_ */
