#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include <LittleFS.h>

CustomCommand *formatCommand = new CustomCommand("format", [](String command)
                                         {
    LittleFS.format();
    ESP.restart();
    return String("{\"event\": \"format\"}"); });