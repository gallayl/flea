#pragma once
#include "CustomCommand.h"
#include "CustomCommands/configAction.h"
#include "CustomCommands/flashlight.h"
#include "CustomCommands/i2c.h"
#include "CustomCommands/info.h"
#include "CustomCommands/move.h"
#include "CustomCommands/pwm.h"
#include "CustomCommands/reset.h"
#include "CustomCommands/servo.h"
#include "CustomCommands/showlog.h"
#include "CustomCommands/wifi.h"

#define COMMANDS_SIZE 128

class CommandInterpreter
{
private:
    static CommandInterpreter *instance;

    uint8_t _registeredCommandsCount = 0;
    void RegisterCommand(CustomCommand newCommand)
    {
        this->RegisteredCommands[this->_registeredCommandsCount] = newCommand;
        this->_registeredCommandsCount++;
    }
    CommandInterpreter()
    {
    }

public:
    String getAvailableCommands()
    {
        String commands = "";

        uint16_t commandId;
        for (commandId = 0; commandId < this->_registeredCommandsCount; commandId++)
        {
            commands += this->RegisteredCommands[commandId].GetCommandName() + ", ";
        };
        return commands;
    }

    String ExecuteCommand(String command)
    {
        for (uint8_t i = 0; i < COMMANDS_SIZE; i++)
        {
            String commandName = this->RegisteredCommands[i].GetCommandName();
            if (command.equals(commandName) || command.startsWith(commandName + " "))
            {
                String result = this->RegisteredCommands[i].Execute(command);
                return result;
            }
        }
        return String("{\"message\": \"Unknown command: " + CommandParser::GetCommandName(command) + ".\", \"availableCommands\": \""+this->getAvailableCommands()+"\"}");
    }

    CustomCommand RegisteredCommands[COMMANDS_SIZE];

    static CommandInterpreter *GetInstance()
    {
        if (instance == 0)
        {
            CommandInterpreter *ci = new CommandInterpreter();
            ci->RegisterCommand(*reset);
            ci->RegisterCommand(*configAction);
            ci->RegisterCommand(*i2cCommand);
            ci->RegisterCommand(*infoAction);
            ci->RegisterCommand(*servoCommand);
            ci->RegisterCommand(*moveAction);
            ci->RegisterCommand(*showLogAction);
            ci->RegisterCommand(*pwmCommand);
            ci->RegisterCommand(*wifiCommand);
            #ifdef ESP32
            ci->RegisterCommand(*flashlightAction);
            #endif
            instance = ci;
        }

        return instance;
    }
};

CommandInterpreter *CommandInterpreter::instance = 0;