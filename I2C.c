#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "I2C.h"

static int i2c_fd = -1;

void i2cSelectAddress(int address)
{
    if (ioctl(i2c_fd, I2C_SLAVE, address) < 0) {
        perror("i2cSelectAddress failed");
    }
}

int i2cInit(int address)
{
    // Open I2C bus
    i2c_fd = open("/dev/i2c-1", O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open /dev/i2c-1");
        return 1;
    }

    // Set slave address
    if (ioctl(i2c_fd, I2C_SLAVE, address) < 0) {
        perror("Failed to set I2C slave address");
        close(i2c_fd);
        return 1;
    }

    return 0;
}

void write8(unsigned char reg, unsigned char value)
{
    unsigned char buf[2];
    buf[0] = reg;
    buf[1] = value;

    if (write(i2c_fd, buf, 2) != 2) {
        perror("I2C write8 failed");
    }
}

unsigned int readU8(unsigned char reg)
{
    unsigned char value;

    // Write register address
    if (write(i2c_fd, &reg, 1) != 1) {
        perror("I2C reg select failed");
        return 0;
    }

    // Read back value
    if (read(i2c_fd, &value, 1) != 1) {
        perror("I2C readU8 failed");
        return 0;
    }

    return value;
}
