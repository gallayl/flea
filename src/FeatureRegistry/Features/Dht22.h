#pragma once
#include "../Feature.h"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <DHT_U.h>
#include "../../CommandInterpreter/CustomCommand.h"
#include "../../CommandInterpreter/CommandInterpreter.h"
#include "../../config.h"
#include "../../services/WebServer.h"

#define DHTPIN D5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);



JsonDocument getDhtDataJson(){

    JsonDocument doc = JsonDocument().as<JsonObject>();
    doc["sensorType"] = "dht22";
    
    doc["temperature"] = dht.readTemperature();
    doc["humidity"] = dht.readHumidity();
    doc["heatIndex"] = dht.computeHeatIndex(false);

    return doc;
}

CustomCommand* ReadDhtData = new CustomCommand("readDht22Data", [](String command){
    JsonDocument doc = getDhtDataJson();
    char buffer[JSON_BUFFER_SIZE];
    serializeJson(doc, buffer);
    return String(buffer);
});

ArRequestHandlerFunction getDht22DataAction = [](AsyncWebServerRequest *request) {
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    JsonDocument info = getDhtDataJson();
    resp->setCode(200);
    resp->getRoot().set(info);
    resp->setLength();
    request->send(resp);
};



Feature *Dht22Feature = new Feature("dht22", [](){
    dht.begin();
    CommandInterpreterInstance->RegisterCommand(*ReadDhtData);
    server.on("/dht22", HTTP_GET, getDht22DataAction);
    return FeatureState::RUNNING;
}, [](){});