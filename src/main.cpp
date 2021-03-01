#include "CommandInterpreter/CommandInterpreter.h"
#include <Wire.h>
#include "./services/Config.h"
#include "./services/Logger.h"
#include "./services/TaskScheduler.h"
#include "./services/FtpServer.h"
#include "./services/WebServer.h"
#include "./services/WebSocketServer.h"
#include "./hw/Camera.h"
#include "./hw/Flashlight.h"
#include "./hw/Pwm.h"
#include "./hw/WiFi.h"

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
    initTaskScheduler();
    initConfig();
    initDisplay();
    initWifi();
    initWebServer();
    initWebSockets();
#ifdef ESP32
    initFlashlight();
    initCamera();
#endif
    initFtpServer();
}

void loop()
{
    runner.execute();
    ftpSrv.handleFTP();
}
