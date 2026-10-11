#include "Encoders.h"

volatile long encoderCountM1 = 0;
volatile long encoderCountM2 = 0;

volatile uint8_t encoderStateM1 = 0;
volatile uint8_t encoderStateM2 = 0;

const float wheelDiameter = 0.062f;
const int pulsesPerRev = 2048;
const float metersPerPulse = (PI * wheelDiameter) / pulsesPerRev;

void initializeEncoders(){
    pinMode(ENC1_A, INPUT_PULLUP);
    pinMode(ENC1_B, INPUT_PULLUP);
    pinMode(ENC2_A, INPUT_PULLUP);
    pinMode(ENC2_B, INPUT_PULLUP);

    encoderStateM1 = (digitalRead(ENC1_A) << 1) | digitalRead(ENC1_B);
    encoderStateM2 = (digitalRead(ENC2_A) << 1) | digitalRead(ENC2_B);

    attachInterrupt(digitalPinToInterrupt(ENC1_A), encoder1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC1_B), encoder1ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC2_A), encoder2ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC2_B), encoder2ISR, CHANGE);
}

void encoder1ISR(){
    uint8_t newState = (digitalRead(ENC1_A) << 1) | digitalRead(ENC1_B);
    uint8_t transition = (encoderStateM1 << 2) | newState;

    switch(transition){
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            encoderCountM1--;
            break;

        case 0b0010:
        case 0b1011:
        case 0b1101:
        case 0b0100:
            encoderCountM1++;
            break;
    }

    encoderStateM1 = newState;
}

void encoder2ISR(){
    uint8_t newState = (digitalRead(ENC2_A) << 1) | digitalRead(ENC2_B);
    uint8_t transition = (encoderStateM2 << 2) | newState;

    switch(transition){
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            encoderCountM2--;
            break;

        case 0b0010:
        case 0b1011:
        case 0b1101:
        case 0b0100:
            encoderCountM2++;
            break;
    }

    encoderStateM2 = newState;
}

float getDistanceM1(){
    noInterrupts();
    long pulses = encoderCountM1;
    interrupts();
    return pulses * (PI * wheelDiameter) / pulsesPerRev;
}

float getDistanceM2(){
    noInterrupts();
    long pulses = encoderCountM2;
    interrupts();
    return pulses * (PI * wheelDiameter) / pulsesPerRev;
}
