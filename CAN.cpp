#include "CAN.h"
#include "Motors.h"
#include "HallSensor.h"
#include "Encoders.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> canBus;

void initializeCAN(){ canBus.begin(); canBus.setBaudRate(500000); Serial.println("CAN initialized");}

bool sendMotorCommand(uint8_t motor, uint8_t direction, uint8_t pwm){
    CAN_message_t msg;

    msg.id = 0x100;
    msg.len = 3;

    msg.buf[0] = motor;
    msg.buf[1] = direction;
    msg.buf[2] = pwm;

    return canBus.write(msg);
}

void receiveBaseCommand(const CAN_message_t &msg) {
  Serial.print("ID RX: ");
  Serial.println(msg.id, HEX);

  for(int i=0;i<msg.len;i++){
      Serial.print(msg.buf[i], HEX);
      Serial.print(" ");
  }
  Serial.println();

  if(msg.id == 0x101){
    if(msg.len >= 1){
        switch(msg.buf[0]){
            case 0:
                referenceAngle = readAngleDegrees();
                Serial.println("Rest position saved");
                break;
            case 1:
                restModeActive = true;
                Serial.println("Returning to rest position");
                break;
        }
    }
    return;
  }
  
  if(msg.id == 0x100){
    if(msg.len < 4) return;

    restModeActive = false; 

    uint8_t dirM1 = msg.buf[0];
    uint8_t pwmM1 = msg.buf[1];
    if(invertMotor1){if(dirM1 == 1) dirM1 = 2; else if(dirM1 == 2) dirM1 = 1;}

    uint8_t dirM2 = msg.buf[2];
    uint8_t pwmM2 = msg.buf[3];

    Serial.println("Motor 1: Dir="); Serial.print(dirM1); Serial.print(" PWM="); Serial.println(pwmM1);
    Serial.println("Motor 2: Dir="); Serial.print(dirM2); Serial.print(" PWM="); Serial.println(pwmM2);

    switch(dirM1){
        case 0: controlMotor(1,STOP); break;
        case 1: moveMotorWithStartup(1,FORWARD,pwmM1); break;
        case 2: moveMotorWithStartup(1,REVERSE,pwmM1); break;
        case 3: controlMotor(1,BRAKE,pwmM1); break;
    }

    switch(dirM2){
        case 0: controlMotor(2,STOP); break;
        case 1: moveMotorWithStartup(2,FORWARD,pwmM2); break;
        case 2: moveMotorWithStartup(2,REVERSE,pwmM2); break;
        case 3: controlMotor(2,BRAKE,pwmM2); break;
    }
  }
  return;
}

void processCAN(){ CAN_message_t msg; while (canBus.read(msg)) {receiveBaseCommand(msg);}}

bool sendDistancesCAN(){
    CAN_message_t msg;

    msg.id = 0x200;
    msg.len = 8;

    int32_t distM1 = getDistanceM1() * 1000.0f;
    int32_t distM2 = getDistanceM2() * 1000.0f;

    memcpy(&msg.buf[0], &distM1, 4);
    memcpy(&msg.buf[4], &distM2, 4);

    return canBus.write(msg);
}

void updateCANTelemetry(){
    static uint32_t lastTransmission = 0;

    if(millis() - lastTransmission > 100){
        lastTransmission = millis();
        sendDistancesCAN();
        sendAngleCAN();
    }
}
bool sendAngleCAN(){
    CAN_message_t msg;

    msg.id = 0x201;
    msg.len = 2;

    float angle = getRelativeAngle();
    int16_t canAngle = angle * 10.0f;

    msg.buf[0] = canAngle & 0xFF;
    msg.buf[1] = (canAngle >> 8) & 0xFF;

    return canBus.write(msg);
}

bool sendSpeedsCAN() {
    static long lastCountM1 = 0;
    static long lastCountM2 = 0;
    static uint32_t lastTime = millis();

    uint32_t now = millis();
    float dt = (now - lastTime) / 1000.0f;
    if(dt <= 0.0f){return false;}

    long deltaM1 = encoderCountM1 - lastCountM1;
    long deltaM2 = encoderCountM2 - lastCountM2;

    float speedM1 = (deltaM1 * metersPerPulse) / dt;
    float speedM2 = (deltaM2 * metersPerPulse) / dt;

    lastCountM1 = encoderCountM1;
    lastCountM2 = encoderCountM2;
    lastTime = now;

    CAN_message_t msg;
    msg.id = 0x202;
    msg.len = 8;

    int32_t speedM1_mm_s = (int32_t)(speedM1 * 1000.0f);
    int32_t speedM2_mm_s = (int32_t)(speedM2 * 1000.0f);

    memcpy(&msg.buf[0], &speedM1_mm_s, 4);
    memcpy(&msg.buf[4], &speedM2_mm_s, 4);

    return canBus.write(msg);
}
