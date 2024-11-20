#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Feature.h"

typedef void (*LogListener)(String, String);

#define LOG_LISTENERS_COUNT 10

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

    void AddListener(LogListener listener)
    {
        this->listeners[this->listenersCount] = listener;
        this->listenersCount++;
    }

private:
    static Logger *instance;

    JsonArray entries;

    byte listenersCount;
    LogListener listeners[LOG_LISTENERS_COUNT];

    void handle(String severity, String message)
    {
        this->addEntry(severity, message);
        Serial.print(severity);
        Serial.print(F(": "));
        Serial.println(message);
    }

    void addEntry(String severity, String message)
    {
        JsonObject entry = this->entries.add<JsonObject>();
        entry["severity"] = severity;
        entry["message"] = message;
    }

    Logger() {}
};

Logger *Logger::instance = nullptr;

Feature *Logging = new Feature("Logging", []()
                               { Logger::GetInstance(); }, []() {});


