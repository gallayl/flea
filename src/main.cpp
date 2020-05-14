#include "CommandInterpreter/CommandInterpreter.h"
#include "McuServer.h"
#include <Wire.h>
#include "./services/Config.h"
#include "./services/Logger.h"
#include "./services/TaskScheduler.h"
#include "./services/FtpServer.h"
#include "./hw/Camera.h"
#include "./hw/Flashlight.h"
#include "./hw/Pwm.h"
#include "./hw/WiFi.h"


int16_t throttleValue = 0;
int16_t steerValue = 0;

CommandInterpreter *interpreter = CommandInterpreter::GetInstance();
McuServer server = McuServer((char *)"admin", (char *)"admin", interpreter);

void setup()
{
    Wire.begin(GPIO_NUM_14, GPIO_NUM_15);
    initPwm();
    Serial.begin(9600);
    initTaskScheduler();
    initConfig();
    initDisplay();
    initWifi();
    server.setup();
    esp_register_shutdown_handler([]() {
        logInfo("Shutting down...");
    });
    initFlashlight();
    initFtpServer();
    initCamera();
    logInfo(String("Wifi: " + WiFi.SSID() + "\nIP: " + WiFi.localIP().toString()));
}

void loop()
{
    runner.execute();
    ftpSrv.handleFTP();
    if (Serial.available() > 0)
    {
        char c[] = {(char)Serial.read()};
        interpreter->ExecuteCommand(c);
    }
}
