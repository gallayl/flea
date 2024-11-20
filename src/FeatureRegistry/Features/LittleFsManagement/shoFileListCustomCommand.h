#pragma once

#include "../../../CommandInterpreter/CustomCommand.h"
#include "../../../api/list.h"
#include "../Logging.h"

#define LOG_BUFFER_LENGTH 1024

CustomCommand *showFileListCustomCommand = new CustomCommand("list", [](String command)
                                                             {
    char buffer[LOG_BUFFER_LENGTH];
    JsonDocument response = LoggerInstance->getEntries();
    serializeJson(response, buffer);
    return String(buffer); });
