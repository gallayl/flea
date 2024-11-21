#pragma once
#include <Arduino.h>
#include <AsyncJson.h>  
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include "../Feature.h"
#include "../../CommandInterpreter/CustomCommand.h"
#include "../../CommandInterpreter/CommandInterpreter.h"
#include "../../services/WebServer.h"

typedef void (*LogListener)(String, String);

extern AsyncWebServer server;

#define LOG_LISTENERS_COUNT 10

class Logger
{
public:

    JsonDocument getEntries()
    {
        return this->entries;
    }

    void Info(String message)
    {
        this->handle("INFO", message);
    }

    void Error(String message)
    {
        this->handle("ERROR", message);
    }

    void Debug(String message)
    {
        this->handle("DEBUG", message);
    }

    void AddListener(LogListener listener)
    {
        this->listeners[this->listenersCount] = listener;
        this->listenersCount++;
    }

    Logger() {
        this->entries = JsonDocument().to<JsonArray>();
        this->listenersCount = 0;
    }


private:

    JsonDocument entries;

    byte listenersCount;
    LogListener listeners[LOG_LISTENERS_COUNT];

    void handle(String severity, String message)
    {
        this->addEntry(severity, message);
        Serial.print(severity);
        Serial.print(F(": "));
        Serial.println(message);
        for (byte i = 0; i < this->listenersCount; i++)
        {
            this->listeners[i](severity, message);
        }
    }

    void addEntry(String severity, String message)
    {
        JsonObject entry = this->entries.add<JsonObject>();
        entry["severity"] = severity;
        entry["message"] = message;
    }

};

Logger *LoggerInstance = new Logger();

#define LOG_BUFFER_LENGTH 1024

CustomCommand *showLogCustomCommand = new CustomCommand("showLog", [](String command)
                                                 {
    char buffer[LOG_BUFFER_LENGTH];
    JsonDocument response = LoggerInstance->getEntries();
    serializeJson(response, buffer);
    return String(buffer); });

ArRequestHandlerFunction showLogRequestHandler = [](AsyncWebServerRequest *request)
{
    AsyncJsonResponse * resp = new AsyncJsonResponse();
    JsonDocument entries = LoggerInstance->getEntries();
    resp->setCode(200);
    resp->getRoot().set(entries);
    resp->setLength();
    request->send(resp);
};

Feature *LoggingFeature = new Feature("Logging", []()                               {
    CommandInterpreterInstance->RegisterCommand(*showLogCustomCommand);
    server.on("/log", HTTP_GET, showLogRequestHandler);
    return FeatureState::RUNNING;
                                }, []() {});
