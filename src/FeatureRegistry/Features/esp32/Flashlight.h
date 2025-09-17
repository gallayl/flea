#pragma once
#ifdef ESP32

#include <AsyncJson.h>  

#include "../../Feature.h"
#include "../../../CommandInterpreter/CustomCommand.h"
#include "../../../CommandInterpreter/CommandInterpreter.h"
#include "../../../CommandInterpreter/CommandParser.h"
#include "../../../services/WebServer.h"

#define FLASHLIGHT_MAX_PWM_FREQ 255
#define FLASH_PIN 4
#define FLASH_PWM_CHANNEL 0


void flashlightOff() {
    ledcWrite(FLASH_PWM_CHANNEL, 0);
}

void flashlightOn() {
    ledcWrite(FLASH_PWM_CHANNEL, FLASHLIGHT_MAX_PWM_FREQ);
}

void setFlashlightDuty(uint32_t duty){
    ledcWrite( FLASH_PWM_CHANNEL, duty);
}

uint32_t  getFlashlightDuty(){
    return ledcRead( FLASH_PWM_CHANNEL);
}

CustomCommand *flashlightAction = new CustomCommand("flashlight", [](String command) {
    String param = CommandParser::GetCommandParameter(command, 1);
    if (param == "on" || param == "true")
    {
        flashlightOn();
    }
    else if (param == "off" || param == "false")
    {
        flashlightOff();
    }
    else if (param.toInt() || param.toInt() == 0){
        setFlashlightDuty(param.toInt());
    }
    return String("");
});

ArRequestHandlerFunction getLights = ([](AsyncWebServerRequest *request)
{
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    uint32_t front = getFlashlightDuty();
    resp->setCode(200);
    resp->getRoot()["front"] = front;
    resp->setLength();
    request->send(resp); });


ArBodyHandlerFunction setLights = ([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{

    JsonDocument doc;
    DeserializationError deserializationError = deserializeJson(doc, (char*)data);
    if (deserializationError)
    {
        request->send(400, MIME_json, deserializationError.f_str());
        return;
    }

    if (doc["front"].is<int>()){
        setFlashlightDuty(doc["front"].as<int>());
        return getLights(request);
    }

    
    request->send(500, MIME_json, F("{\"success\":false}")); });

Feature* FlashlightFeature = new Feature("flashlight", [](){
    ledcSetup(FLASH_PWM_CHANNEL,5000,8);
    ledcAttachPin(FLASH_PIN, FLASH_PWM_CHANNEL);
    CommandInterpreterInstance->RegisterCommand(*flashlightAction);


        server.on("/lights", HTTP_GET, getLights);
        server.on("/lights", HTTP_POST, getLights, NULL, setLights);

    return FeatureState::RUNNING;
}, [](){
});

#endif