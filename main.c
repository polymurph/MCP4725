//#include <msp430.h>
#include "HAL/msp430fr6989.h"

#include "HAL/hal_i2c.h"
//#include "HWO/mcp4725.h"

#define DAC_ADDRESS 0b11000000


int main(void)
{
    uint16_t i = 0;
    uint8_t data[2] = {
                    0b00100000,
                    0b00000000
    };





    //hal_i2c_init(i2c_mode_MASTER, i2c_clk_src_ACLK, 10);

    UCB0



    hal_i2c_write(DAC_ADDRESS, data, 2);





	while(1)
	{
	    for(i = 0; i < 0xFFFF; i++);
	    hal_i2c_write(DAC_ADDRESS, data, 2);
	    for(i = 0; i < 0xFFFF; i++);
	    hal_i2c_write(DAC_ADDRESS, data, 2);

	}
	
	return 0;
}
