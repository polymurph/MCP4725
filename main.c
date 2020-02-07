//#include <msp430.h>
#include "HAL/msp430fr6989.h"


// info: https://electronics.stackexchange.com/questions/67524/msp430-i%C2%B2c-single-write-read-example

#include <stdint.h>
#include "HAL/hal_clk.h"
//#include "HAL/hal_i2c.h"
//#include "HWO/mcp4725.h"

#define DAC_ADDRESS 0b01100010

void _i2c_init_master()
{
    // enable register manipulations
    UCB0CTLW0 |= UCSWRST;

    // 7 bit slave address mode
    UCB0CTLW0 &= ~UCSLA10;

    // single master mode
    UCB0CTLW0 &= ~UCMM;

    // set master mode
    UCB0CTLW0 |= UCMST;

    // set i2c mode
    UCB0CTLW0 |= UCMODE_3;

    // needed for master mode. read 23.3.5.2 Master Mode
    UCB0CTLW0 &= ~UCSYNC;

    // select SMCLK as clock source
    UCB0CTLW0 &= ~UCSSEL_3;
    UCB0CTLW0 |= UCSSEL__SMCLK;

    UCB0BRW = 0x0008;

    // gpios settup for i2c functionality
    // P1.6 SDA
    // P1.7 SCL
    PM5CTL0 &= ~LOCKLPM5;
    P1SEL0 |= 0xC0;
    P1SEL1 &= ~0xC0;
    PM5CTL0 |= LOCKLPM5;

    // dissable manipulations to register and enable i2c operation
    UCB0CTLW0 &= ~UCSWRST;

}


int8_t _i2c_master_tx(uint8_t address, const uint8_t *data, uint8_t len)
{
    if(len == 0) return -1;

    uint8_t index = 0;

    // set slave address
    UCB0I2CSA |= address;

    // setting transmitter mode
    UCB0CTLW0 |= UCTR;

    // generate start condition
    UCB0CTLW0 |= UCTXSTT;

    // poll for UCTXIFG0
    while(!(UCB0IFG & UCTXIFG0))

    // poll for address transmission completion
    while(UCB0CTLW0 & UCTXSTT);

    do{
        // write data in TX register
        UCB0TXBUF = data[index++];

        // poll for UCTXIFG0
        while(!(UCB0IFG & UCTXIFG0));

    }while(index < len);

    // generate stop condition
    UCB0CTLW0 |= UCTXSTP;

    return 0;
}

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable Watchdog

    hal_clk_config_SMCLK(clk_SMCLK_src_LFXT, clk_presc_DIV_1, true, true);

    uint8_t data_1[2] = { 0b00001000,
                  0b00000000

    };

    uint8_t data_2[2] = { 0b00000000,
                  0b00000000

    };

    _i2c_init_master();

	while(1)
	{
	    _i2c_master_tx(DAC_ADDRESS, data_1, 2);
        __delay_cycles(10000);
	    _i2c_master_tx(DAC_ADDRESS, data_2, 2);
        __delay_cycles(10000);

	}
	
	return 0;
}


