#include "MotionHistory.h"

long motor1PulseHistory[HISTORY_SIZE] = {0};
long motor2PulseHistory[HISTORY_SIZE] = {0};

float motor1OutputRevolutionsHistory[HISTORY_SIZE] = {0};
float motor2OutputRevolutionsHistory[HISTORY_SIZE] = {0};

uint8_t motor1Index = 0;
uint8_t motor2Index = 0;

// Functions ------------------------------------------------------------------------------------------------

void printLastMovement(uint8_t motor)
{
    long pulses;
    float outputRevolutions;

    if(motor == 1){
        uint8_t last =(motor1Index + HISTORY_SIZE - 1) % HISTORY_SIZE;
        pulses = motor1PulseHistory[last];
        outputRevolutions = motor1OutputRevolutionsHistory[last];
    }
    else if(motor == 2){
        uint8_t last = (motor2Index + HISTORY_SIZE - 1) % HISTORY_SIZE;
        pulses = motor2PulseHistory[last];
        outputRevolutions = motor2OutputRevolutionsHistory[last];
    }
    else{return;}

    float motorRevolutions = pulses / CPR;

    Serial.print("Motor ");
    Serial.println(motor);
    Serial.print("Pulses: ");
    Serial.println(pulses);

    Serial.print("Motor revolutions: ");
    Serial.println(motorRevolutions, 4);
    Serial.print("Output shaft revolutions: ");
    Serial.println(outputRevolutions, 4);

    Serial.println("---------------------");
}

void saveMovement(uint8_t motor, long pulses){
    float outputRevolutions = pulses / OUTPUT_CPR;

    if(motor == 1){
        motor1PulseHistory[motor1Index] = pulses;
        motor1OutputRevolutionsHistory[motor1Index] = outputRevolutions;
        motor1Index = (motor1Index + 1) % HISTORY_SIZE;
    }
    else if(motor == 2){
        motor2PulseHistory[motor2Index] = pulses;
        motor2OutputRevolutionsHistory[motor2Index] = outputRevolutions;
        motor2Index = (motor2Index + 1) % HISTORY_SIZE;
    }
}

void printHistory(uint8_t motor){
    Serial.print("Motor History ");
    Serial.println(motor);

    for(uint8_t i = 0; i < HISTORY_SIZE; i++){
        long pulses;
        float outputRevolutions;

        if(motor == 1){
            pulses = motor1PulseHistory[i];
            outputRevolutions = motor1OutputRevolutionsHistory[i];
        }
        else if(motor == 2){
            pulses = motor2PulseHistory[i];
            outputRevolutions = motor2OutputRevolutionsHistory[i];
        }
        else{return;}

        Serial.print(i);
        Serial.print(" -> Pulses: ");
        Serial.print(pulses);
        Serial.print(" | Motor revolutions: ");
        Serial.print(pulses / CPR, 4);
        Serial.print(" | Output shaft revolutions: ");
        Serial.println(outputRevolutions, 4);
    }
    Serial.println("---------------------");
}