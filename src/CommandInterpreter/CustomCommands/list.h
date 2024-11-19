#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../api/list.h"
#include "../../services/Logger.h"

CustomCommand *showFileListCustomCommand = new CustomCommand("list", [](String command)
                                                             {
    char buffer[LOG_BUFFER_LENGTH];
    JsonDocument response = getFileList();
    serializeJson(response, buffer);
    return String(buffer); });
