#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../services/Config.h"

CustomCommand *configAction = new CustomCommand("config", [](String command) {
    String operation = CommandParser::GetCommandParameter(command, 1);

    if (!operation.compareTo("reload"))
    {
        initConfig();
    }

    if (!operation.compareTo("reset"))
    {
        setDefaultConfig();
        saveConfigFile();
    }

    if (!operation.compareTo("set"))
    {
        String key = CommandParser::GetCommandParameter(command, 2);
        String value = CommandParser::GetCommandParameter(command, 3);
        if (key && value)
        {
            setConfigValue(key, value);
        }
    }

    char buffer[CONFIG_SIZE];
    serializeJson(configJson, buffer);
    return String(buffer);
});
