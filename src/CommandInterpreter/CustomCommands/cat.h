#pragma once

#include "NewPing.h"
#include "LittleFS.h"
#include "../CommandParser.h"
#include "../CustomCommand.h"

CustomCommand *catAction = new CustomCommand("cat", [](String command) {
    String param = CommandParser::GetCommandParameter(command, 1);
    String result = "";
    File f = LittleFS.open(param, "r");
    if (!f)
    {
        return String("Failed to open file.");
    }
    return f.readString();
});
