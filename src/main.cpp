#include "CommandInterpreter/CommandInterpreter.h"
#include <Wire.h>
#include "./services/Config.h"
#include "./services/Logger.h"
#include "./services/WebServer.h"
#include "./services/WebSocketServer.h"
#include "./hw/Camera.h"
#include "./hw/Flashlight.h"
#include "./hw/Pwm.h"
#include "./hw/WiFi.h"
#include "./FeatureRegistry/FeatureRegistry.h"

int16_t throttleValue = 0;
int16_t steerValue = 0;

void setup()
{
#ifdef ESP32
    Wire.begin(GPIO_NUM_14, GPIO_NUM_15);
#else
    Wire.begin();
#endif
    initPwm();
    Serial.begin(9600);
    initConfig();
    initDisplay();
    initWifi();
    initWebServer();
    initWebSockets();
#ifdef ESP32
    initFlashlight();
    initCamera();
#endif
}

void loop()
{
    if (Serial.available())
    {
        String command = Serial.readStringUntil('\n');
        command.replace("\n", "");
        command.replace("\r", "");
        String response = CommandInterpreter::GetInstance()->ExecuteCommand(command);
        Serial.println(response);
    }
}
