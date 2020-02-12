//#include <msp430.h>
#include "HAL/msp430fr6989.h"


// info: https://electronics.stackexchange.com/questions/67524/msp430-i%C2%B2c-single-write-read-example

// https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/702571?MSP430FR5994-I2C-Inaccurate-eUSCI-description-on-User-s-Guide

#include <stdint.h>
#include "HAL/hal_clk.h"
#include "HAL/hal_i2c.h"
#include "HWO/mcp4725.h"

#define POLL_MAX_TIME 10000
//#define DAC_ADDRESS 0b01100010
//#define DAC_ADDRESS 0b0000000
#define DAC_ADDRESS 0b01100000

static inline void _initiate_start_condition()
{
    // generate start condition
    UCB0CTLW0 |= UCTXSTT;

    // poll for UCTXIFG0
    while(!(UCB0IFG & UCTXIFG0));
}

static inline void _initiate_stop_condition()
{
    UCB0CTLW0 |= UCTXSTP;
    while(UCB0CTLW0 & UCTXSTP);
    UCB0IFG  &= ~UCTXSTP;
}

static inline int8_t _check_max_poll_time()
{
    static uint16_t timer = POLL_MAX_TIME;

    if(timer > 0) {
        timer--;
        return 0;
    } else {
        return 1;
    }
}

void _i2c_init_master()
{
#ifdef example
    UCB1CTLW0 |= UCSWRST;       // Reset I2C interface for configuration


    UCB0CTLW0 =                 /* USCI - B1 configuration register */
                UCMST         | // Master mode
                UCSYNC        | // Synchronous mode
                UCMODE_3      | // I2C mode
                UCSSEL__SMCLK | // Select SMCLK
                UCTR          | // Transmitter mode
                UCSWRST       | // Don't release reset (yet)
                0;


    UCB0CTLW1 =
                UCCLTO_0      | // Clock low time-out select (disabled)
                UCASTP_0      | // Automatic STOP condition generation (disabled)
                UCGLIT_0      | // Deglitch time (50ns)
                0;


    UCB0BRW = 10;               // Bit clock divider 1M/10 = 100kHz


    /*UCB0I2COA0 = OWN_ADDRESS  | // Own address
                 UCOAEN       | // Address Enable
                 0;
     */

    // gpios settup for i2c functionality
    // P1.6 SDA
    // P1.7 SCL
    PM5CTL0 &= ~LOCKLPM5;
    P1SEL0 |= 0xC0;
    P1SEL1 &= ~0xC0;
    PM5CTL0 |= LOCKLPM5;

    UCB0CTLW0 &= ~UCSWRST;
#endif // example

#define my_version
#ifdef my_version
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

    // dissable automatic stop generation
    UCB1CTLW1 &= ~UCASTP_3;

    // clock prescaler
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
#endif // my_version
}


int8_t _i2c_master_tx(uint8_t address, const uint8_t *data, uint8_t len)
{
    if(len == 0) return -1;

    // clear old flags
    UCB0IFG = 0;

    // set slave address
    UCB0I2CSA = address;

    // setting transmitter mode
    UCB0CTLW0 |= UCTR;

    _initiate_start_condition();

    // poll for address transmission completion
    while(UCB0CTLW0 & UCTXSTT);

    // TODO: check for NACK here!


    do{
        // write data in TX register
        UCB0TXBUF = *data;
        data++;

        while( !(UCB0IFG & UCTXIFG) & !(UCB0IFG & UCNACKIFG) );

        /*
        while(!(UCB0IFG & UCTXIFG0)){
            if(_check_max_poll_time()) {
                _initiate_stop_condition();
                return -1;
            }
        }*/

        if(UCB0IFG & UCNACKIFG) {
            UCB0IFG &= ~UCNACKIFG;
            _initiate_stop_condition();
            return -1;
        }

        // poll for UCTXIFG0
        //while(!(UCB0IFG & UCTXIFG0));
        //while(UCB0IFG & UCTXIFG0);
        //UCB0IFG &= ~UCTXIFG0;

    }while(len--);

    _initiate_stop_condition();

    return 0;
}




#define ramp_example


#ifdef example_
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable Watchdog
    volatile int8_t status = 0;

    hal_clk_config_SMCLK(clk_SMCLK_src_LFXT, clk_presc_DIV_1, true, true);

    uint8_t data_1[2] = { 0b00001000,
                  0b00000000

    };

    uint8_t data_2[2] = { 0b00000000,
                  0b00000000

    };

    uint8_t data_3[2] = { 0b00001111,
                  0b11111111

    };

    _i2c_init_master();
    //hal_i2c_init(i2c_mode_MASTER, i2c_clk_src_SMCLK, 0x0008);

    while(1)
    {
        status = _i2c_master_tx(DAC_ADDRESS, data_3, 2);
        //status = hal_i2c_write(DAC_ADDRESS, data_3, 2);
        __delay_cycles(10000);
        status = _i2c_master_tx(DAC_ADDRESS, data_2, 2);
        __delay_cycles(10000);

    }

    return 0;
}
#endif // example_

#ifdef example_0
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable Watchdog
    volatile int8_t status = 0;

    hal_clk_config_SMCLK(clk_SMCLK_src_LFXT, clk_presc_DIV_1, true, true);

    uint8_t data_1[2] = { 0b00001000,
                  0b00000000

    };

    uint8_t data_2[2] = { 0b00000000,
                  0b00000000

    };

    uint8_t data_3[2] = { 0b00001111,
                  0b11111111

    };

    //_i2c_init_master();
    hal_i2c_init(i2c_mode_MASTER, i2c_clk_src_SMCLK, 0x0008);

	while(1)
	{
	    //status = _i2c_master_tx(DAC_ADDRESS, data_3, 2);
	    status = hal_i2c_write(DAC_ADDRESS, data_3, 2);
        __delay_cycles(10000);
	    status = _i2c_master_tx(DAC_ADDRESS, data_2, 2);
        __delay_cycles(10000);

	}
	
	return 0;
}
#endif // example_0

#ifdef ramp_example
int main(void)
{
    uint16_t cnt = 0;
    mcp4725_t dac;

    WDTCTL = WDTPW + WDTHOLD; // Disable Watchdog

    hal_clk_config_DCO(clk_dco_freq_4_MHz);
    hal_clk_config_SMCLK(clk_SMCLK_src_DCO, clk_presc_DIV_1, true, true);


    //hal_clk_config_SMCLK(clk_SMCLK_src_LFXT, clk_presc_DIV_1, true, true);
    hal_i2c_init(i2c_mode_MASTER, i2c_clk_src_SMCLK, 0x0080);

    mcp4725_init(&dac,
                 hal_i2c_write,
                 mcp4725_addr_0x0,
                 mcp4725_pwrd_md_NORMAL,
                 0,
                 0);

    while(1)
    {
        mcp4725_write_DAC(&dac, cnt++);
        cnt &= 0x0FFFF;
        //__delay_cycles(1000);


    }
}
#endif // example_1

#ifdef example_2
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Disable Watchdog
    volatile int8_t status = 0;

    //hal_clk_config_DCO(clk_dco_freq_1_MHz);
    hal_clk_config_SMCLK(clk_SMCLK_src_LFXT, clk_presc_DIV_1, true, true);



    uint8_t data_1[2] = { 0b00001000,
                  0b00000000

    };

    uint8_t data_2[2] = { 0b00000000,
                  0b00000000

    };

    uint8_t data_3[2] = { 0b00001111,
                  0b11111111

    };

    //_i2c_init_master();
    hal_i2c_init(i2c_mode_MASTER, i2c_clk_src_SMCLK, 0x0008);

    while(1)
    {
        //status = _i2c_master_tx(DAC_ADDRESS, data_3, 2);
        status = hal_i2c_write(DAC_ADDRESS, data_3, 2);
        __delay_cycles(10000);
        status = _i2c_master_tx(DAC_ADDRESS, data_2, 2);
        __delay_cycles(10000);

    }

    return 0;
}
#endif // example_2
