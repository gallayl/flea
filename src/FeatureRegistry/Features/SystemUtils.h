#include <ArduinoJson.h>
#include "../Feature.h"
#include "./wifiCommand.h"
#include "./configCustomCommand.h"
#include "../../CommandInterpreter/CommandInterpreter.h"
#include "../../CommandInterpreter/CustomCommand.h"
#include "../FeatureRegistry.h"

CustomCommand *resetCommand = new CustomCommand("restart", [](String command) {
    ESP.restart();
    return String("{\"event\": \"restart\"}");
});


Feature *SystemFeatures = new Feature("SystemFeatures", []() {
    CommandInterpreterInstance->RegisterCommand(*wifiCommand);
    CommandInterpreterInstance->RegisterCommand(*resetCommand);
    CommandInterpreterInstance->RegisterCommand(*configCustomCommand);
}, []() {
    
});