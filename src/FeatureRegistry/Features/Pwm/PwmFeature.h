
#pragma once
#include "./pwmInstance.h"
#include "../../Feature.h"
#include "../Logging.h"
#include "../../../CommandInterpreter/CommandInterpreter.h"
#include "./pwmCustomCommand.h"
#include "./servoCustomCommand.h"
#include "./moveCustomCommand.h"



Feature *PwmFeature = new Feature("PwmFeature", []() {
    bool success = pwm.begin(B000000);
    if (!success)
    {
        LoggerInstance->Error(F("PWM not available"));
        return FeatureState::ERROR;
    }
    CommandInterpreterInstance->RegisterCommand(*moveCustomCommand);
    CommandInterpreterInstance->RegisterCommand(*pwmCommand);
    CommandInterpreterInstance->RegisterCommand(*servoCommand);
    return FeatureState::RUNNING;
}, []() {
    
});

