#pragma once

#ifdef ESP32
#include "LittleFS.h"
#else
#include "LittleFS.h"
#endif

#include "../CommandParser.h"
#include "../CustomCommand.h"

CustomCommand *formatAction = new CustomCommand("format", [](String command) {
    LittleFS.format();
    return String("Storage formatted");
});
