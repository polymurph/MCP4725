/*
 * hal_i2c.c
 *
 *  Created on: Nov 13, 2019
 *      Author: Edwin Koch
 */

#include "hal_i2c.h"
#include "msp430fr6989.h"

#include <stdbool.h>
#include <stdint.h>

static inline void _initiate_start_condition();

static inline void _initiate_stop_condition();

static inline void _stop_sequence()
{
    UCB1CTLW0 |= UCTXSTP;
    while(!(UCB1CTLW0 & UCTXSTP));
}

#if 0
static inline void _poll_busy()
{
    while(UCB1STATW & UCBBUSY);
}
#endif

static inline bool _start_sequence(uint8_t address, bool read)
{
    UCB1IFG = 0;    // clear old flags

    UCB1I2CSA = address;    // set slave address

    // set read/write mode
    if(read){
        // read
        UCB1CTLW0 &= ~UCTR;
    } else {
        // write
        UCB1CTLW0 |= UCTR;
    }

    // generate start condition
    UCB1CTLW0 |= UCTXSTT;

    // wait until slave address has been sent
    while(UCB1CTLW0 & UCTXSTT);

    // TODO: wait for ACK from Slave here?

    if(read){
        if(UCB1IFG & UCTXIFG0) return false;
    } else {
        if(UCB1IFG & UCRXIFG0) return false;
    }

    return true;


#if 0
    UCB1IFG = 0;    // clear old flags
    UCB1I2CSA = address;    // set slave address
    UCB1CTLW0 |= (read) ? (UCTR) : (0); // set send/recive mode
    UCB1CTLW0 |= UCTXSTT;   // generate start condition
    while(!(UCB1CTLW0 & UCTXSTT)); // wait until start condition is sent

    // check for error
    if(UCB1IFG & UCNACKIFG){
        _stop_sequence();
        return true;
    }
    return false;
#endif
}

static inline bool _write_tx_buf(uint8_t data)
{
    UCB1TXBUF = data;
    while(!(UCB1IFG & UCTXIFG0));   // wait for transmit completion

#if 0
    // check for error
    if(UCB1IFG & UCNACKIFG){
        _stop_sequence();
        return true;
    }
#endif
    return false;
}

static inline uint8_t _read_rx_buf()
{
    while(!(UCB1IFG & UCRXIFG0));
    return UCB1RXBUF;
}

static inline void _gpio_settup()
{

    // gpios settup for i2c functionality
    // P1.6 SDA
    // P1.7 SCL
    PM5CTL0 &= ~LOCKLPM5;
    P1SEL0 |= 0xC0;
    P1SEL1 &= ~0xC0;
    PM5CTL0 |= LOCKLPM5;
#ifdef old
    PM5CTL0 &= ~LOCKLPM5;

    // settup GPIOs for i2c mode
    // P4DIR &= ~0x03; // SDA and SCL as output
    //P4SEL0 &= ~0x03;
    P4SEL0 |= 0x03;
    P4SEL1 |= 0x03;
#endif // old
}


void hal_i2c_init(i2c_mode_t        i2c_mode,
                  i2c_clk_src_t     source,
                  uint16_t          bitrate)
{

    // enable register manipulations
    UCB0CTLW0 |= UCSWRST;

    // 7 bit slave address mode
    UCB0CTLW0 &= ~UCSLA10;

    // single master mode
    UCB0CTLW0 &= ~UCMM;

    // set master mode
    UCB0CTLW0 |= i2c_mode;

    // set i2c mode
    UCB0CTLW0 |= UCMODE_3;

    // needed for master mode. read 23.3.5.2 Master Mode
    UCB0CTLW0 &= ~UCSYNC;

    // select SMCLK as clock source
    UCB0CTLW0 &= ~UCSSEL_3;
    UCB0CTLW0 |= source;

    // dissable automatic stop generation
    UCB1CTLW1 &= ~UCASTP_3;

    // clock prescaler
    UCB0BRW = bitrate;

    _gpio_settup();

    // dissable manipulations to register and enable i2c operation
    UCB0CTLW0 &= ~UCSWRST;

#ifdef old
    // set reset condition
    UCB1CTLW0 |= UCSWRST;

    // master/slave mode
    UCB1CTLW0 |= i2c_mode;

    // synchronous mode
    UCB1CTLW0 |= UCSYNC;

    // set bitrate
    UCB0BRW = bitrate;

    _gpio_settup();

    // clear settings
    UCB1CTLW0 &= 0xFFFE;




#if 0
    // set reset condition
    UCB1CTLW0 |= UCSWRST;
    _gpio_settup();

    // set eUSCI_B mode to i2c
    UCB1CTLW0 |= (UCMODE1_H | UCMODE0_H);
    UCB1CTLW0 |= UCSYNC;

    // mode set to master
    UCB1CTLW0 |= UCMST;

    // single master environment
    UCB1CTLW0 &= ~UCMM;

    // 7 bit address mode
    UCB1CTLW0 &= ~UCSLA10;

    // select clock source
    UCB1CTLW0 &= ~UCSSEL_3;
    UCB1CTLW0 |= source;

    // set bit rate
    UCB1BRW = bitrate;

    UCB1CTLW1 = UCASTP_2;

    // erase reset condition -> release for operation
    UCB1CTLW0 &= ~UCSWRST;
#endif
#endif // old
}

void hal_i2c_setClockSource(i2c_clk_src_t source)
{
    // unlock register
    UCB1CTLW0 |= UCSWRST;
    UCB1CTLW0 &= ~(UCSSEL0 | UCSSEL1);
    UCB1CTLW0 |= source;
    // lock register
    UCB1CTLW0 &= ~UCSWRST;
}

void hal_i2c_setClockPrescaler(uint16_t prescaler)
{
    // unlock register
    UCB1CTLW0 |= UCSWRST;
    UCB1BRW = prescaler;
    // lock register
    UCB1CTLW0 &= ~UCSWRST;
}

void hal_i2c_write_Byte(uint8_t address, uint8_t data)
{
    _start_sequence(address, false);
    _write_tx_buf(data);
    _stop_sequence();
}

void hal_i2c_write_Register(uint8_t address, uint8_t reg, uint8_t data)
{
    _start_sequence(address, false);
    _write_tx_buf(reg);
    _write_tx_buf(data);
    _stop_sequence();
}

i2c_error_t hal_i2c_write(uint8_t address, const uint8_t * data, uint8_t len)
{
    if(len == 0) return i2c_error_DATA_LEN_IS_0;

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
    if(UCB0IFG & UCNACKIFG) {
        UCB0IFG &= ~UCNACKIFG;
        _initiate_stop_condition();
        return i2c_error_DEVICE_NOT_FOUND;
    }

    do{
        // write data in TX register
        UCB0TXBUF = *data;
        data++;

        // polling for transmission completion
        while(!(UCB0IFG & UCTXIFG) & !(UCB0IFG & UCNACKIFG));

        if(UCB0IFG & UCNACKIFG) {
            UCB0IFG &= ~UCNACKIFG;
            _initiate_stop_condition();
            return i2c_error_RECIVED_NACK;
        }
    }while(len--);

    _initiate_stop_condition();

    return i2c_error_NONE;
    /*
    uint8_t i = 0;
    if(_start_sequence(address, false)) return true;
    for(i = 0; i < len; i++){
        if(_write_tx_buf(data[i])) return true;
    }
    _stop_sequence();
    return i2c_error_NONE;
    */
}

bool hal_i2c_read_Byte(uint8_t address, uint8_t *byte)
{
    if(_start_sequence(address, true)) return true;
    *byte = _read_rx_buf();
    _stop_sequence();
    return false;
}

void hal_i2c_read(uint8_t address, uint8_t * data, uint8_t len)
{

}


//////////

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

