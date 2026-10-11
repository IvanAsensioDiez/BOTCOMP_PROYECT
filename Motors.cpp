#include "Motors.h"

void initializeMotors(){
    pinMode(M1_IN1, OUTPUT);
    pinMode(M1_IN2, OUTPUT);
    pinMode(M2_IN1, OUTPUT);
    pinMode(M2_IN2, OUTPUT);

    controlMotor(1, STOP);
    controlMotor(2, STOP);
}

void moveMotorWithStartup(uint8_t motor, MotorMode mode, uint8_t pwm){
    controlMotor(motor, mode, 200);
    delay(15);
    controlMotor(motor, mode, pwm);
}

void controlMotor(uint8_t motor, MotorMode mode, uint8_t speed){
    uint8_t in1;
    uint8_t in2;

    if(speed >= 255){speed = 255;}
    if(motor == 1)     {in1 = M1_IN1; in2 = M1_IN2;}
    else if(motor == 2){in1 = M2_IN1; in2 = M2_IN2;}
    else{Serial.println("Invalid motor"); return;}

    switch(mode){
        case FORWARD: analogWrite(in1, speed); analogWrite(in2, 0); break;
        case REVERSE: analogWrite(in1, 0); analogWrite(in2, speed); break;
        case BRAKE:   analogWrite(in1, 255); analogWrite(in2, 255);break;
        case STOP: default: analogWrite(in1, 0); analogWrite(in2, 0); break;
    }
}