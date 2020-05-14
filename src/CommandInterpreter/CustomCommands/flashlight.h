#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../hw/Flashlight.h"


CustomCommand *flashlightAction = new CustomCommand("flashlight", [](String command) {
    String param = CommandParser::GetCommandParameter(command, 1);
    if (param == "on" || param == "true")
    {
        flashlightOn();
    }
    else if (param == "off" || param == "false")
    {
        flashlightOff();
    }
    else if (param.toInt() || param.toInt() == 0){
        setFlashlightDuty(param.toInt());
    }
    return String("");
});
