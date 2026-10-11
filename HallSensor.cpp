#include "HallSensor.h"
#include "Motors.h"

float referenceAngle = 0.0f;
bool invertMotor1 = false;
bool restModeActive = false;

void initializeHallSensor(){Wire.begin();}

uint8_t readStatus(){
    Wire.beginTransmission(HALL_ADDR);
    Wire.write(0x0B);
    Wire.endTransmission(false);
    Wire.requestFrom(HALL_ADDR, 1);

    if(Wire.available()){return Wire.read();}return 0;
}

uint16_t readHallAngle(){
    Wire.beginTransmission(HALL_ADDR);
    Wire.write(0x0C); // Angle register
    Wire.endTransmission(false);
    Wire.requestFrom(HALL_ADDR, 2);

    if(Wire.available() >= 2){
        uint8_t msb = Wire.read();
        uint8_t lsb = Wire.read();

        uint16_t rawValue = ((uint16_t)msb << 8) | lsb;
        uint16_t angle = rawValue & 0x0FFF;

        return angle;
    }
    return 0;
}

float readAngleDegrees(){uint16_t value = readHallAngle(); return (value * 360.0f) / 4096.0f;}

void updateOrientation(){
    float angle = getRelativeAngle();

    if     (!invertMotor1 && (angle > 100 || angle < -100)) {invertMotor1 = true;}
    else if(invertMotor1 &&  (angle < 80 && angle > -80))   {invertMotor1 = false;}
}

void setRestPosition(){referenceAngle = readAngleDegrees();}

float getRelativeAngle(){
    float angle = readAngleDegrees();
    float relativeAngle = angle - referenceAngle;

    while(relativeAngle > 180)  {relativeAngle -= 360;}
    while(relativeAngle < -180) {relativeAngle += 360;}

    return relativeAngle;
}

void returnToRestPosition(){
    float currentAngle = readAngleDegrees();
    float error = referenceAngle - currentAngle;

    // Shortest angular path
    while(error > 180)  {error -= 360;}
    while(error < -180) {error += 360;}

    if(abs(error) < 5) {controlMotor(2, STOP); return;}

    uint8_t pwm = constrain(abs(error) * 2, 80, 255);

    Serial.println(error);

    if     (error > 15)  {moveMotorWithStartup(2, REVERSE, pwm); return;}
    else if(error < -15) {moveMotorWithStartup(2, FORWARD, pwm); return;}

    if(error > 0)      {controlMotor(2, REVERSE, 160);}
    else if(error < 0) {controlMotor(2, FORWARD, 160);}
}