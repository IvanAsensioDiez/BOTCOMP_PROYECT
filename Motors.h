#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

#define M1_IN1 2
#define M1_IN2 3
#define M2_IN1 4
#define M2_IN2 5

enum MotorMode{STOP,FORWARD,REVERSE,BRAKE};

void initializeMotors();
void controlMotor(uint8_t motor, MotorMode mode, uint8_t speed = 255);
void moveMotorWithStartup(uint8_t motor, MotorMode mode, uint8_t pwm);

#endif