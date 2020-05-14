#pragma once

#include "../hw/Flashlight.h"

ArRequestHandlerFunction setLights = ([](AsyncWebServerRequest *request) {
    int params = request->params();
    for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        String variable = p->name();
        String value = p->value();
        uint32_t val = constrain(value.toInt(),1,255);
        if (!variable.compareTo("front")){
            setFlashlightDuty(val);
            request->send(200, "application/json", String("Front set to:") + String(val));
            return;
        }
    }
    request->send(500, "application/json", F("{\"success\":false}"));
});