/*
 * I2C.cpp
 *
 *  Created on: Jun 9, 2020
 *      Author: Edwin
 */

#include "I2C.h"
#include "stm32f4xx_hal.h"

I2C::I2C() {

	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		while(1);
	}
}

I2C::~I2C() {

}

void I2C::write(uint8_t address, uint8_t *data, uint8_t n) {
	HAL_I2C_Master_Transmit(&hi2c1, address, data, n, 1000);
}

//// private

