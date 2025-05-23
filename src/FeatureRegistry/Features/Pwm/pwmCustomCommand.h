#pragma once

#include "../../../CommandInterpreter/CommandParser.h"
#include "../../../CommandInterpreter/CustomCommand.h"
#include "./pwmInstance.h"

#define PWM_COMMAND_INPUT_SIZE 64


CustomCommand *pwmCommand = new CustomCommand("pwm", [](String fullCommand) {
    char input[PWM_COMMAND_INPUT_SIZE + 1];
    CommandParser::GetCommandParameter(fullCommand, 1).toCharArray(input, fullCommand.length());
    byte size = fullCommand.length();
    // Add the final 0 to end the C string
    input[size] = 0;
    String result = "{\"event\": \"pwmChange\", \"details\": [";
    // Read each command pair
    char *command = strtok(input, ";");
    while (command != 0)
    {
        // Split the command in two values
        char *separator = strchr(command, '=');
        if (separator != 0)
        {
            // Actually split the string in 2: replace ':' with 0
            *separator = 0;
            int channel = constrain(atoi(command), 0, 15);
            ++separator;
            long end = constrain(atol(separator), 0, 4095);
            pwm.setPWM(channel, 0, end);
            result += "{\"channel\":";
            result += channel;
            result += ", \"end\":";
            result += end;
            result += "},";
        }
        command = strtok(0, ";");
    }
    result = result.substring(0, result.length() - 1);
    result += "]}";

    return result;
});