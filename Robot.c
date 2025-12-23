#include "Robot.h"
#include "I2C.h"
#include <unistd.h>
#include <stdio.h>

/* ADDRESSES */
#define MOTOR_ADDR 0x60
#define SERVO_ADDR 0x40

/* SERVO CONFIG */
#define SERVO_CHANNEL 0
#define SERVO_LEFT    380
#define SERVO_CENTER  270
#define SERVO_RIGHT   150

/* MOTOR CONTROL */

void robotForward()
{
    i2cSelectAddress(MOTOR_ADDR);
    setPWMFreq(1600);

    // LEFT side (inverted)
    run(BACKWARD, 3);
    run(BACKWARD, 1);

    // RIGHT side
    run(FORWARD, 2);
    run(FORWARD, 0);
}

void robotStop()
{
    i2cSelectAddress(MOTOR_ADDR);
    setPWMFreq(1600);

    for (int i = 0; i < 4; i++) {
        setPWM(motors[i].pwm, 0, 0);
        setPin(motors[i].in1, 0);
        setPin(motors[i].in2, 0);
    }
}

void robotLeft()
{
    i2cSelectAddress(MOTOR_ADDR);
    setPWMFreq(1600);

    run(FORWARD, 2);
    run(FORWARD, 3);

    run(BACKWARD, 0);
    run(BACKWARD, 1);
}

void robotRight()
{
    i2cSelectAddress(MOTOR_ADDR);
    setPWMFreq(1600);

    run(FORWARD, 0);
    run(FORWARD, 1);

    run(BACKWARD, 2);
    run(BACKWARD, 3);
}

/* SERVO CONTROL */

static void servoSet(int pulse)
{
    i2cSelectAddress(SERVO_ADDR);
    setPWMFreq(50);

    setPWM(SERVO_CHANNEL, 0, pulse);
    usleep(400000);
}

void lookCenter() { servoSet(SERVO_CENTER); }
void lookLeft()   { servoSet(SERVO_LEFT); }
void lookRight()  { servoSet(SERVO_RIGHT); }
