#include <Wire.h>
#include "./CommandInterpreter/CommandInterpreter.h"
#include "./services/WebServer.h"
#include "./services/WebSocketServer.h"
#include "./hw/WiFi.h"
#include "./FeatureRegistry/FeatureRegistry.h"

int16_t throttleValue = 0;
int16_t steerValue = 0;

void setup()
{
    Serial.begin(9600);

    Serial.println("Starting up FLEA...");

    initWifi();
    initWebServer();
    initWebSockets();

    FeatureRegistryInstance->SetupFeatures();
}

void loop()
{
    FeatureRegistryInstance->LoopFeatures();
}
