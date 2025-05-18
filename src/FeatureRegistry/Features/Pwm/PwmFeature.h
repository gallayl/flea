
#pragma once
#include "./pwmInstance.h"
#include "../../Feature.h"
#include "../Logging.h"
#include "../../../CommandInterpreter/CommandInterpreter.h"
#include "./pwmCustomCommand.h"
#include "./servoCustomCommand.h"
#include "./moveCustomCommand.h"

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

Feature *PwmFeature = new Feature("PwmFeature", []()
                                  {
    bool success = pwm.begin();
    if (!success)
    {
        LoggerInstance->Error(F("PWM not available"));
        return FeatureState::ERROR;
    }

    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

    CommandInterpreterInstance->RegisterCommand(*moveCustomCommand);
    CommandInterpreterInstance->RegisterCommand(*pwmCommand);
    CommandInterpreterInstance->RegisterCommand(*servoCommand);
    return FeatureState::RUNNING; }, []() {

                                  });
