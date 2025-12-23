#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef I2C

#define I2C

/*
This function is needed to initialize the communication with 
the i2c bus.  This function should be the first function that
is called within your project. Only the address is needed for
this function.  The buss variable is for those that feel they
need to duplicate what they see in the python file.  It does
nothing for this function. 
*/
int i2cInit(int address);

void i2cSelectAddress(int address);

/*This function is needed to write data to the hat*/
void write8(unsigned char reg, unsigned char value);

/*This function is needed to read data from the hat*/
unsigned int readU8(unsigned char reg);


#endif
