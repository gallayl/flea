#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "../Services/TaskScheduler.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

#define DIM_TIMEOUT 5000
#define TURN_OFF_TIMEOUT 30000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool isDisplayAvailable = false;

void dimCallback(){
    display.dim(true);
    display.display();
}

void turnOffCallback(){
    display.clearDisplay();
    display.display();
}

Task dimTask(5000, 1, &dimCallback);
Task turnOffTask(30000, 1, &turnOffCallback);

void displayInfo(String message) {

    display.dim(false);
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(message);
    display.display();
    // dimTask.enable();
    dimTask.restartDelayed(DIM_TIMEOUT);
    turnOffTask.restartDelayed(TURN_OFF_TIMEOUT);
    // turnOffTask.enable();
}

void initDisplay() {
    runner.addTask(dimTask);
    runner.addTask(turnOffTask);
    dimTask.disable();
    turnOffTask.disable();
    isDisplayAvailable = display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
    display.dim(false);
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    displayInfo(F("Starting..."));
};


