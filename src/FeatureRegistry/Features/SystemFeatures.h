#include <ArduinoJson.h>
#include "../registeredFeatures.h"
#include "../Feature.h"
#include "./wifiCommand.h"
#include "../../CommandInterpreter/CommandInterpreter.h"
#include "../../CommandInterpreter/CustomCommand.h"
#include "../FeatureRegistry.h"
#include "../../services/WebServer.h"

CustomCommand *resetCommand = new CustomCommand("restart", [](String command) {
    ESP.restart();
    return String("{\"event\": \"restart\"}");
});

CustomCommand *getRegisteredFeatures = new CustomCommand("getRegisteredFeatures", [](String command) {
    char output[1024];
    serializeJson(registeredFeatures, output);
    return String(output);
});

ArRequestHandlerFunction getFeatures = [](AsyncWebServerRequest *request) {
    char output[1024];
    serializeJson(registeredFeatures, output);
    request->send(200, "application/json", output);
};


ArRequestHandlerFunction reset = [](AsyncWebServerRequest *request) {
    ESP.restart();
    request->send(200, "application/json", "{\"event\": \"restart\"}");
};


Feature *SystemFeatures = new Feature("SystemFeatures", []() {
    CommandInterpreterInstance->RegisterCommand(*wifiCommand);
    CommandInterpreterInstance->RegisterCommand(*resetCommand);
    CommandInterpreterInstance->RegisterCommand(*getRegisteredFeatures);
    server.on("/features", HTTP_GET, getFeatures);
    server.on("/restart", HTTP_POST, reset);
    return FeatureState::RUNNING;
}, []() {
    
});