#ifndef ENCODERS_H
#define ENCODERS_H

#include <Arduino.h>

// Encoder pins
#define ENC1_A 17
#define ENC1_B 16
#define ENC2_A 15
#define ENC2_B 14

extern volatile long encoderCountM1;
extern volatile long encoderCountM2;

extern const float wheelDiameter;
extern const int pulsesPerRev;
extern const float metersPerPulse;

void initializeEncoders();

void encoder1ISR();
void encoder2ISR();

float getDistanceM1();
float getDistanceM2();

#endif
