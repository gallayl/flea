#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../services/Logger.h"

CustomCommand *showLogAction = new CustomCommand("showLog", [](String command)
                                                 {
    char buffer[LOG_BUFFER_LENGTH];
    JsonDocument response = getLogResponse();
    serializeJson(response, buffer);
    return String(buffer); });
