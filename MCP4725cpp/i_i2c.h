#ifndef _I_I2C_H_
#define _I_I2C_H_

#include <stdint.h>


class I_I2C
{
    public:
    virtual void write(uint8_t address, uint8_t* data, uint8_t n) = 0;
    private:
};

#endif // _I_I2C_H_




