#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Feature.h"

class Logger
{
public:
    static Logger *GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new Logger();
        }
        return instance;
    }

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

private:
    static Logger *instance;

    JsonArray entries;

    void handle(String severity, String message)
    {
        this->addEntry(severity, message);
        Serial.print(severity);
        Serial.print(F(": "));
        Serial.println(message);
    }

    void addEntry(String severity, String message)
    {
        JsonObject entry = this->entries.createNestedObject();
        entry["severity"] = severity;
        entry["message"] = message;
    }

    Logger() {}
};

Feature *Logging = new Feature("Logging", []()
                               { Logger::GetInstance(); }, []() {});
