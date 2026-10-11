#ifndef HALL_SENSOR_H
#define HALL_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

extern float referenceAngle;
extern bool invertMotor1;
extern bool restModeActive;

#define HALL_ADDR 0x36

void initializeHallSensor();
uint8_t readStatus();
uint16_t readHallAngle();
float readAngleDegrees();
void updateOrientation();
void setRestPosition();
float getRelativeAngle();
void returnToRestPosition();

#endif