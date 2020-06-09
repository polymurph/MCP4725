/*
 * I2C.h
 *
 *  Created on: Jun 9, 2020
 *      Author: Edwin
 */

#ifndef I2C_H_
#define I2C_H_

#include "II2C.h"
#include "main.h"

class I2C: public II2C {
public:
	I2C();
	virtual ~I2C();

	void write(uint8_t address, uint8_t *data, uint8_t n);

private:
	I2C_HandleTypeDef hi2c1;
};

#endif /* I2C_H_ */
