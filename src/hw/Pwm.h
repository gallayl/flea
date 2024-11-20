#pragma once

#include "../FeatureRegistry/Features/Logging.h"
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN 100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 550 // this is the 'maximum' pulse length count (out of 4096)

#define PWM_COMMAND_INPUT_SIZE 64

Adafruit_PWMServoDriver pwm;

void initPwm() {
    Logger::GetInstance()->Info(F("Initializing PWM..."));
    pwm.begin(B000000);
}

uint8_t getServoPulse(long degrees){
    return constrain(map(degrees, 0, 180, SERVOMIN, SERVOMAX), SERVOMIN, SERVOMAX);
}

int setServoDegree(int channel, long degrees){
    return pwm.setPWM(channel, 0, getServoPulse(degrees));
}