#ifndef ROBOT_H
#define ROBOT_H

#include "MotorHat.h"

/* Movement */
void robotForward();
void robotStop();
void robotLeft();
void robotRight();

/* Servo look functions */
void lookCenter();
void lookLeft();
void lookRight();

#endif
