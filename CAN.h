#ifndef CAN_H
#define CAN_H

#include <Arduino.h>
#include <FlexCAN_T4.h>

enum MotorDirection{MOTOR_STOP = 0, MOTOR_FORWARD = 1, MOTOR_REVERSE = 2};

void initializeCAN();
bool sendMotorCommand(uint8_t motor, uint8_t direction, uint8_t pwm);
void processBaseCommand(const CAN_message_t &msg);
void processCAN();
bool sendDistancesCAN();
void updateCANTelemetry();
bool sendAngleCAN();
bool sendSpeedsCAN();

#endif
