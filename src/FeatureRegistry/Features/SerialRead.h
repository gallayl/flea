#pragma once
#include <Arduino.h>
#include "../Feature.h"
#include "../../CommandInterpreter/CommandInterpreter.h"


Feature *SerialReadFeature = new Feature("SerialRead", []() {}, []()
                                  {
    if (Serial.available())
    {
        String command = Serial.readStringUntil('\n');
        command.replace("\n", "");
        command.replace("\r", "");
        String response = CommandInterpreterInstance->ExecuteCommand(command);
        Serial.println(response);
    } });