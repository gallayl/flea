#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Feature.h"
#include "../../CommandInterpreter/CustomCommand.h"
#include "../../CommandInterpreter/CommandInterpreter.h"

typedef void (*LogListener)(String, String);

#define LOG_LISTENERS_COUNT 10

class Logger
{
public:

    JsonArray getEntries()
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
        this->entries = JsonArray();
        this->listenersCount = 0;
    }


private:

    JsonArray entries;

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

CustomCommand *showLogAction = new CustomCommand("showLog", [](String command)
                                                 {
    char buffer[LOG_BUFFER_LENGTH];
    JsonDocument response = LoggerInstance->getEntries();
    serializeJson(response, buffer);
    return String(buffer); });


Feature *LoggingFeature = new Feature("Logging", []()                               {
    // CommandInterpreterInstance->RegisterCommand(*showLogAction);
                                }, []() {});
