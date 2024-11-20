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
    pwm.sleep();
}

void move(int speed, int steer)
{
    pwm.wakeup();
    pwm.setPWM(BACK_LEFT_CHANNEL, 0, constrain((BACK_LEFT_CENTER + speed), BACK_LEFT_MAX_FORWARD, BACK_LEFT_MAX_BACKWARD));
    pwm.setPWM(BACK_RIGHT_CHANNEL, 0, constrain((BACK_RIGHT_CENTER - speed), BACK_RIGHT_MAX_FORWARD, BACK_RIGHT_MAX_BACKWARD));
    pwm.setPWM(STEER_CHANNEL,0, constrain((STEER_CENTER - steer), STEER_MAX_RIGHT, STEER_MAX_LEFT));
}