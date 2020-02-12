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


//#define MCP4725_ADDRESS (MCP4725_ADDRESS_BASE | MCP4725_A0_BIT)

#if 0
typedef void (*callback_t)();
typedef uint8_t (*callback_1_t)(uint8_t);
#endif


typedef uint8_t (*u8_fptr_u8_pu8_u8_t)(uint8_t,const uint8_t*, uint8_t);
/*
typedef uint8_t (*u8_fptr_u8_pu8_u8_t)(uint8_t, *uint8_t, uint8_t);
*/

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

enum{
	MCP4725_DAC_REG_BIT_PD0 = 0x10,
	MCP4725_DAC_REG_BIT_PD1	= 0x20,
	MCP4725_DAC_REG_BIT_C0 = 0x20,
	MCP4725_DAC_REG_BIT_C1 = 0x40,
	MCP4725_DAC_REG_BIT_C2 = 0x80,
};

typedef enum {
    mcp4725_cmd_FAST_MODE = 0x00,
    mcp4725_cmd_WRITE_DAC = 0x40,
    mcp4725_cmd_WRITE_DAC_AND_EEPROM = 0x60
}mcp4725_cmd_t;

typedef enum{
	NORMAL_MODE = 0,
	ZA_1k_ohm = 0x01,
	ZA_100k_ohm = 0x02,
	ZA_500k_ohm = 0x03
}mcp4725_pdwn_imp_t;

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
    uint16_t dac_data;
    uint16_t eemprom_data;
}mcp4725_t;

#if 0
void mcp4725_register_i2c_start(callback_1_t start);
void mcp4725_regsiter_i2c_write(callback_1_t write);
void mcp4725_register_i2c_stop(callback_t stop);
#endif

void mcp4725_init(mcp4725_t* device,
                  u8_fptr_u8_pu8_u8_t i2c_tx_cb,
                  mcp4725_addr_t address,
                  mcp4725_pwrd_md_t power_down_mode,
                  uint16_t dac_data,
                  uint16_t eemprom_data);

void mcp4725_write_DAC(const mcp4725_t* device, uint16_t value);

void mcp4725_write_DAC_and_EEPROM(const mcp4725_t* device, uint16_t value);

void mcp4725_set_powerdown_impedance(mcp4725_t* device, mcp4725_pdwn_imp_t impedance);



#ifdef old

void mcp4725_init(u8_fptr_u8_pu8_u8 i2c_tx);

void mcp4725_write_DAC(uint8_t address, uint16_t data);

void mcp4725_eeprom_write(uint8_t address, uint16_t data);

void mcp4725_set_powerdown_impedance(mcp4725_pdwn_imp_t imp);

#endif // old


#endif /* MCP4725_H_ */
