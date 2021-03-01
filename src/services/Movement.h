#pragma once

#include "../hw/Pwm.h"

#define STEER_CHANNEL 0
#define STEER_CENTER 450
#define STEER_MAX_LEFT 520
#define STEER_MAX_RIGHT 360

#define BACK_LEFT_CHANNEL 2
#define BACK_LEFT_CENTER 350
#define BACK_LEFT_MAX_FORWARD 100
#define BACK_LEFT_MAX_BACKWARD 660

#define BACK_RIGHT_CHANNEL 1
#define BACK_RIGHT_CENTER 350
#define BACK_RIGHT_MAX_FORWARD 100
#define BACK_RIGHT_MAX_BACKWARD 660

#define YAW_CHANNEL 14
#define YAR_CENTER = 450
#define YAW_LEFT = 680

void initMovement()
{
}

void stopMoving()
{
    pwm.setAllChannelsPWM(0);
}

void move(int speed, int steer)
{
    uint16_t pwms[3];
    pwms[BACK_LEFT_CHANNEL] = constrain((BACK_LEFT_CENTER + speed), BACK_LEFT_MAX_FORWARD, BACK_LEFT_MAX_BACKWARD);
    pwms[BACK_RIGHT_CHANNEL] = constrain((BACK_RIGHT_CENTER - speed), BACK_RIGHT_MAX_FORWARD, BACK_RIGHT_MAX_BACKWARD);
    pwms[STEER_CHANNEL] = constrain((STEER_CENTER - steer), STEER_MAX_RIGHT, STEER_MAX_LEFT);
    pwm.setChannelsPWM(0, 3, pwms);
}