#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../services/Logger.h"


CustomCommand *showLogAction = new CustomCommand("showLog", [](String command) {
    char buffer[LOG_BUFFER_LENGTH];
    StaticJsonDocument<LOG_BUFFER_LENGTH> response = getLogResponse();
    serializeJson(response, buffer);
    return String(buffer);
});
