#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "./pwm.h"

#define SERVOMIN 100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 550 // this is the 'maximum' pulse length count (out of 4096)

#define PWM_COMMAND_INPUT_SIZE 64

CustomCommand *servoCommand = new CustomCommand("servo", [](String fullCommand) {
    char input[PWM_COMMAND_INPUT_SIZE + 1];
    CommandParser::GetCommandParameter(fullCommand, 1).toCharArray(input, fullCommand.length());
    byte size = fullCommand.length();
    // Add the final 0 to end the C string
    input[size] = 0;
    String result = "{\"event\": \"servoChange\", \"details\": [";
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
            long degrees = atol(separator);
            setServoDegree(channel, degrees);
            result += "{\"channel\":";
            result += channel;
            result += ", \"degrees\":";
            result += degrees;
            result += "},";
        }
        command = strtok(0, ";");
    }
    result = result.substring(0, result.length() - 1);
    result += "]}";

    return result;
});