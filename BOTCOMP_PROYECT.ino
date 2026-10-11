#include <Wire.h>
#include "Motors.h"
#include "MotionHistory.h"
#include "Encoders.h"
#include "HallSensor.h"
#include "CAN.h"

// Motor encoder
const float CPR = 48.0f;
const float OUTPUT_CPR = 4741.44f;
const float GEAR_RATIO = 98.78f;
uint32_t lastTelemetry = 0;

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    initializeCAN();
    Serial.println("CAN TEST START");

    initializeMotors();
    initializeEncoders();
    initializeHallSensor();

    Serial.println("System initialized");
    Serial.print("MAGNETIC ENCODER STATUS = ");

    Serial.println(readStatus(), BIN);
}

void loop()
{
    processCAN();

    if(millis() - lastTelemetry >= 100){
        lastTelemetry = millis();
        sendSpeedsCAN();
    }
    updateOrientation();
    updateCANTelemetry();

    if(restModeActive){returnToRestPosition();}
}