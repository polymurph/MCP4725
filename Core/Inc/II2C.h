/*
 * II2C.h
 *
 *  Created on: Jun 9, 2020
 *      Author: Edwin
 */

#ifndef II2C_H_
#define II2C_H_
#include <stdint.h>

class II2C {
public:
	virtual void write(uint8_t address, uint8_t data, uint8_t n) = 0;
};

#endif /* II2C_H_ */
