#include "MotorHat.h"

motor motors[4];

void initMotors()
{
    for (int i = 0; i < 4; i++) {
        setPin(motors[i].in1, 0);
        setPin(motors[i].in2, 0);
        setPWM(motors[i].pwm, 0, 0);
    }
}


void initHat()
{
    // Motor 1
    motors[0].pwm = 8;
    motors[0].in2 = 9;
    motors[0].in1 = 10;

    // Motor 2
    motors[1].pwm = 13;
    motors[1].in2 = 12;
    motors[1].in1 = 11;

    // Motor 3
    motors[2].pwm = 2;
    motors[2].in2 = 3;
    motors[2].in1 = 4;

    // Motor 4
    motors[3].pwm = 7;
    motors[3].in2 = 6;
    motors[3].in1 = 5;
}


void setPin(unsigned char pin, unsigned char value)
{
    if (value)
        setPWM(pin, 4096, 0);
    else
        setPWM(pin, 0, 4096);
}

void run(unsigned char command, unsigned char motorID)
{
    int speed;

    // Right motors faster
    if (motorID == 0 || motorID == 1) {
        speed = 1500;   // RIGHT side
    } else {
        speed = 600;   // LEFT side
    }

    if (command == FORWARD) {
        setPin(motors[motorID].in1, 1);
        setPin(motors[motorID].in2, 0);
        setPWM(motors[motorID].pwm, 0, speed);
    }
    else if (command == BACKWARD) {
        setPin(motors[motorID].in1, 0);
        setPin(motors[motorID].in2, 1);
        setPWM(motors[motorID].pwm, 0, speed);
    }
    else { // STOP / RELEASE
        setPWM(motors[motorID].pwm, 0, 0);
        setPin(motors[motorID].in1, 0);
        setPin(motors[motorID].in2, 0);
    }
}

