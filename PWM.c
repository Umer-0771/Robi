#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "PWM.h"

void setAllPWM(int on, int off)
{
    write8(__ALL_LED_ON_L, on & 0xFF);
    write8(__ALL_LED_ON_H, on >> 8);
    write8(__ALL_LED_OFF_L, off & 0xFF);
    write8(__ALL_LED_OFF_H, off >> 8);
}

void setPWM(int channel, int on, int off)
{
    int base = __LED0_ON_L + 4 * channel;
    write8(base, on & 0xFF);
    write8(base + 1, on >> 8);
    write8(base + 2, off & 0xFF);
    write8(base + 3, off >> 8);
}

void setPWMFreq(int freq)
{
    float prescaleval = 25000000.0;
    prescaleval /= 4096.0;
    prescaleval /= freq;
    prescaleval -= 1.0;

    int prescale = (int)(prescaleval + 0.5);

    int oldmode = readU8(__MODE1);
    write8(__MODE1, (oldmode & 0x7F) | __SLEEP);
    usleep(5000);

    write8(__PRESCALE, prescale);
    usleep(5000);

    write8(__MODE1, oldmode);
    usleep(5000);
    write8(__MODE1, oldmode | __RESTART);
}

void PWMInit(unsigned int address)
{
    // Wake up PCA9685
    write8(__MODE1, 0x00);
    usleep(10000);

    write8(__MODE2, __OUTDRV);   // OPEN-DRAIN REQUIRED FOR SERVO
    write8(__MODE1, __ALLCALL);
    usleep(10000);

    setAllPWM(0, 0);
}

