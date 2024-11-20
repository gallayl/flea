#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../api/list.h"
#include "../../FeatureRegistry/Features/Logging.h"

#define LOG_BUFFER_LENGTH 1024

CustomCommand *showFileListCustomCommand = new CustomCommand("list", [](String command)
                                                             {
    char buffer[LOG_BUFFER_LENGTH];
    JsonDocument response = Logger::GetInstance()->getEntries();
    serializeJson(response, buffer);
    return String(buffer); });
