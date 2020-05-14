#pragma once

#include "../services/Logger.h"
#include "PCA9685.h"

#define SERVOMIN 100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 550 // this is the 'maximum' pulse length count (out of 4096)

#define PWM_COMMAND_INPUT_SIZE 64

PCA9685 pwm;

void initPwm() {
    logInfo(F("Initializing PWM..."));
    // pwm.resetDevices();
    pwm.init(B000000);
    pwm.setPWMFrequency(60); // Analog servos run at ~60 Hz updates
    // pwm.setAllChannelsPWM(0);
}

long getServoPulse(long degrees){
    return constrain(map(degrees, 0, 180, SERVOMIN, SERVOMAX), SERVOMIN, SERVOMAX);
}

void setServoDegree(int channel, long degrees){
    return pwm.setChannelPWM(channel, getServoPulse(degrees));
}