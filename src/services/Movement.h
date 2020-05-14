#pragma once

#include "../hw/Pwm.h"

#define BACK_LEFT_CHANNEL 0
#define BACK_LEFT_CENTER 95

#define BACK_RIGHT_CHANNEL 2
#define BACK_RIGHT_CENTER 103

#define STEER_CHANNEL 1
#define STEER_CENTER 60

void initMovement(){

}

void stopMoving(){
    pwm.setAllChannelsPWM(0);
}

void move(int speed, int steer) {
    if (speed == 0 && steer == 0){
        stopMoving();
        return;
    }
    uint16_t pwms[3];
    pwms[BACK_LEFT_CHANNEL] = getServoPulse(BACK_LEFT_CENTER + speed);
    pwms[BACK_RIGHT_CHANNEL] = getServoPulse(BACK_RIGHT_CENTER - speed);
    pwms[STEER_CHANNEL] = getServoPulse(STEER_CENTER + steer);
    pwm.setChannelsPWM(0,3, pwms);

    // setServoDegree(BACK_LEFT_CHANNEL, BACK_LEFT_CENTER + speed + steer);
    // setServoDegree(BACK_RIGHT_CHANNEL, BACK_RIGHT_CENTER - speed - steer;);
    // setServoDegree(STEER_CHANNEL, STEER_CENTER + steer);
}