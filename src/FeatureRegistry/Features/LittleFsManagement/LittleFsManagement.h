#pragma once
#include "../../Feature.h"
#include "../../../CommandInterpreter/CommandInterpreter.h"
#include "../Logging.h"
#include "./shoFileListCustomCommand.h"

Feature LittleFsFeatures = Feature("LittleFsFeatures", []() {
    CommandInterpreterInstance->RegisterCommand(*showFileListCustomCommand);
}, []() {
    
});