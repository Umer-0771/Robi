#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "I2C.h"
#include "PWM.h"
#include "MotorHat.h"
#include "Robot.h"
#include "Sensor.h"

#define STOP_DISTANCE_CM 20.0

static void stopServo()
{
    i2cSelectAddress(0x40);   // Servo PCA9685
    setPWMFreq(50);

    // Disable PWM on servo channel
    setPWM(0, 0, 0);
}
/* Ctrl+C Safety Handler */
static void handle_sigint(int sig)
{
    (void)sig;
    printf("\nCtrl-C detected — stopping robot\n");

    // Stop motors
    robotStop();

    // Stop servo
    stopServo();

    // Shut down all PWM outputs (extra safety)
    setAllPWM(0, 0);

    printf("Robot safely shut down\n");
    exit(0);
}

/*  MAIN  */
int main()
{
    /* Register Ctrl+C handler */
    signal(SIGINT, handle_sigint);

    /* Initialize I2C */
    if (i2cInit(0x60) != 0) {
        printf("I2C init failed\n");
        return 1;
    }

    /* Initialize PCA9685 + motors */
    PWMInit(0x60);
    initHat();
    initMotors();
    // Initialize SERVO board ONCE
    i2cSelectAddress(0x40);
    PWMInit(0x40);
    setPWMFreq(50);
    lookCenter();
    usleep(300000);

    // Switch back to MOTOR board
    i2cSelectAddress(0x60);
    setPWMFreq(1600);


    /* Initialize ultrasonic sensor */
    sensorInit();

    /* Center servo at startup */
    lookCenter();
    printf("Centering servo\n");
    usleep(300000);
    stopServo();

    /* Main Control Loop */
    while (1) {

        float front = getDistanceCM();

        /* Ignore invalid readings */
        if (front < 2 || front > 400) {
            usleep(100000);
            continue;
        }

        printf("Front: %.2f cm\n", front);

        /* Path is clear → go forward */
        if (front > STOP_DISTANCE_CM) {
            robotForward();
            usleep(100000);
            continue;
        }

        /* Obstacle detected */
        printf("Obstacle detected — scanning\n");
        robotStop();
        usleep(200000);

        /* Look RIGHT */
        lookRight();
        usleep(200000);
        float right = getDistanceCM();
        printf("Right: %.2f cm\n", right);

        /* Look LEFT */
        lookLeft();
        usleep(200000);
        float left = getDistanceCM();
        printf("Left: %.2f cm\n", left);

        /* Center servo again */
        lookCenter();
        usleep(200000);

        /* Decide direction */
        if (right > left) {
            robotRight();
            printf("Turning RIGHT\n");
        } else {
            robotLeft();
            printf("Turning LEFT\n");
        }

        usleep(400000);
        robotStop();
    }
}

