#pragma once 
#include "../../CommandInterpreter/CommandInterpreter.h"
#include "../../CommandInterpreter/CustomCommand.h"
#include "../../CommandInterpreter/CommandParser.h"
#include "../Feature.h"
#include "../FeatureRegistry.h"
#include "./Logging.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

#define DIM_TIMEOUT 5000
#define TURN_OFF_TIMEOUT 30000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

CustomCommand *displayCommand = new CustomCommand("display", [](String command) {
    String operation = CommandParser::GetCommandParameter(command, 1);
    return String(F("Not implemented"));
});

Feature* DisplayFeature = new Feature("Display", []() {
    bool success = display.begin(SSD1306_SWITCHCAPVCC, 0x3c);

    if (!success) {
        LoggerInstance->Error(F("Display not available"));
        return FeatureState::ERROR;
    }

    display.dim(false);
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text

    CommandInterpreterInstance->RegisterCommand(*displayCommand);

    LoggerInstance->AddListener([](String severity, String message) {
    display.dim(false);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(message);
    display.display();
    });
    return FeatureState::RUNNING;

}, []() {
    
});


