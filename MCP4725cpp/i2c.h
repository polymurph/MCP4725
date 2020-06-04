#ifndef _I2C_H_
#define _I2C_H_
#include "i_i2c.h"

class I2C : public I_I2C
{
    public:
    void write(uint8_t address, uint8_t* data, uint8_t n);
    private:

};

#endif // _I2C_H_