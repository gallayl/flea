#pragma once

#include <ArduinoJson.h>
#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../FeatureRegistry/Features/Logging.h"

#define LOG_BUFFER_LENGTH 1024

CustomCommand *showLogAction = new CustomCommand("showLog", [](String command)
                                                 {
    char buffer[LOG_BUFFER_LENGTH];
    JsonDocument response = Logger::GetInstance()->getEntries();
    serializeJson(response, buffer);
    return String(buffer); });
