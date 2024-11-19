#pragma once

#include "../hw/Display.h"
#include <ArduinoJson.h>

#define LOG_ENTRIES 128
#define LOG_BUFFER_LENGTH 2048

String logBuffer[LOG_ENTRIES];
int logPointer = 0;

void logInfo(String message)
{
    if (isDisplayAvailable)
    {
        displayInfo(message);
    }
    Serial.println(message);
    logBuffer[logPointer] = message;
    if (logPointer++ > LOG_ENTRIES)
    {
        logPointer = 0;
    };
}

JsonDocument getLogResponse()
{
    JsonDocument response;
    response["pointer"] = logPointer;
    JsonArray entries = response["entries"].to<JsonArray>();
    for (int i = logPointer; i < LOG_ENTRIES; i++)
    {
        if (logBuffer[i] != NULL && logBuffer[i].length() > 0)
        {
            entries.add(logBuffer[i]);
        }
    }
    for (int i = 0; i < logPointer; i++)
    {
        if (logBuffer[i] != NULL && logBuffer[i].length() > 0)
        {
            entries.add(logBuffer[i]);
        }
    }

    return response;
}