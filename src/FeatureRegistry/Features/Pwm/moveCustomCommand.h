#pragma once

#include <ArduinoJson.h>
#include "../../../CommandInterpreter/CommandParser.h"
#include "../../../CommandInterpreter/CustomCommand.h"
#include "./Movement.h"

#define MOTOR_COMM

CustomCommand *moveCustomCommand = new CustomCommand("move", [](String fullCommand)
                                              {
    JsonDocument response;

    String param1 = CommandParser::GetCommandParameter(fullCommand, 1);
    if (param1.compareTo("stop") == 0)
    {
        stopMoving();
    }

    String param2 = CommandParser::GetCommandParameter(fullCommand, 2);

    int speed = param1.toInt();
    int steer = param2.toInt();

    move(speed, steer);

    response["speed"] = speed;
    response["steer"] = steer;

    char buffer[128];
    serializeJson(response, buffer);
    return String(buffer); });
