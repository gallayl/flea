#include <ArduinoJson.h>
#include "../registeredFeatures.h"
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

CustomCommand *getRegisteredFeatures = new CustomCommand("getRegisteredFeatures", [](String command) {
    char output[1024];
    serializeJson(registeredFeatures, output);
    return String(output);
});


Feature *SystemFeatures = new Feature("SystemFeatures", []() {
    CommandInterpreterInstance->RegisterCommand(*wifiCommand);
    CommandInterpreterInstance->RegisterCommand(*resetCommand);
    CommandInterpreterInstance->RegisterCommand(*configCustomCommand);
    CommandInterpreterInstance->RegisterCommand(*getRegisteredFeatures);
    return FeatureState::RUNNING;
}, []() {
    
});