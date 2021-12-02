#pragma once

#include "NewPing.h"
#include "LittleFS.h"
#include "../CommandParser.h"
#include "../CustomCommand.h"

CustomCommand *lsAction = new CustomCommand("ls", [](String command) {
    String result = "";
    Dir dir = LittleFS.openDir("/");
    while (dir.next())
    {
        result += dir.fileName();
        result += " / ";
        result += dir.fileSize();
        result += "\r\n";
    }
    return result;
});
