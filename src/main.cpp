#include <Wire.h>
#include "./CommandInterpreter/CommandInterpreter.h"
#include "./services/Config.h"
#include "./services/WebServer.h"
#include "./services/WebSocketServer.h"
#include "./hw/Flashlight.h"
#include "./hw/WiFi.h"
#include "./FeatureRegistry/FeatureRegistry.h"

int16_t throttleValue = 0;
int16_t steerValue = 0;

void setup()
{
    Serial.begin(9600);
#ifdef ESP32
    Wire.begin(GPIO_NUM_14, GPIO_NUM_15);
#else
    Wire.begin();
#endif
    initConfig();
    initWifi();
    initWebServer();
    initWebSockets();
#ifdef ESP32
    initFlashlight();
#endif

    FeatureRegistryInstance->SetupFeatures();

}

void loop()
{
    if (Serial.available())
    {
        String command = Serial.readStringUntil('\n');
        command.replace("\n", "");
        command.replace("\r", "");
        String response = CommandInterpreterInstance->ExecuteCommand(command);
        Serial.println(response);
    }
}
