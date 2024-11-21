#pragma once
#ifdef ESP32


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

ArRequestHandlerFunction setLights = ([](AsyncWebServerRequest *request)
                                      {
    int params = request->params();
    for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        String variable = p->name();
        String value = p->value();
        uint32_t val = constrain(value.toInt(),1,255);
        if (!variable.compareTo("front")){
            setFlashlightDuty(val);
            request->send(200, MIME_json, String("Front set to:") + String(val));
            return;
        }
    }
    request->send(500, MIME_json, F("{\"success\":false}")); });

Feature* FlashlightFeature = new Feature("flashlight", [](){
    ledcSetup(FLASH_PWM_CHANNEL,5000,8);
    ledcAttachPin(FLASH_PIN, FLASH_PWM_CHANNEL);
    CommandInterpreterInstance->RegisterCommand(*flashlightAction);
        server.on("/lights", HTTP_GET, setLights);

    return FeatureState::RUNNING;
}, [](){
});

#endif