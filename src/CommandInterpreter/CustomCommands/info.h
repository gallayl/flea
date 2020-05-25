#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../hw/Camera.h"
#include "../../hw/Display.h"
#include "../../hw/WiFi.h"
#include "../../services/FtpServer.h"
#include <ArduinoJson.h>
#ifdef ESP32
#include <WiFi.h>
#include <SPIFFS.h>
#else
#include <ESP8266WiFi.h>
#include <FS.h>
#endif

#ifdef ESP32
String getResetReason()
{
    switch (esp_reset_reason())
    {
    case ESP_RST_UNKNOWN:
        return String("Reset reason can not be determined");
    case ESP_RST_POWERON:
        return String("Reset due to power-on event");
    case ESP_RST_EXT:
        return String("Reset by external pin (not applicable for ESP32)");
    case ESP_RST_SW:
        return String("Software reset via esp_restart");
    case ESP_RST_PANIC:
        return String("Software reset due to exception/panic");
    case ESP_RST_INT_WDT:
        return String("Reset (software or hardware) due to interrupt watchdog");
    case ESP_RST_TASK_WDT:
        return String("Reset due to task watchdog");
    case ESP_RST_WDT:
        return String("Reset due to other watchdogs");
    case ESP_RST_DEEPSLEEP:
        return String("Reset after exiting deep sleep mode");
    case ESP_RST_BROWNOUT:
        return String("Brownout reset (software or hardware)");
    case ESP_RST_SDIO:
        return String("Reset over SDIO");
    }
    return String("Unknown");
}
#endif


CustomCommand *infoAction = new CustomCommand("info", [](String command) {
    StaticJsonDocument<512> response;

    JsonObject esp = response.createNestedObject("esp");

    esp["sdkVersion"] = ESP.getSdkVersion();
    esp["cpuFreqMhz"] = ESP.getCpuFreqMHz();
    esp["freeHeap"] = ESP.getFreeHeap();
    esp["freeSkSpace"] = ESP.getFreeSketchSpace();

    JsonObject flash = response.createNestedObject("flash");

    flash["mode"] = ESP.getFlashChipMode();
    flash["size"] = ESP.getFlashChipSize();
    flash["speed"] = ESP.getFlashChipSpeed();

    #ifdef ESP32
    esp["restartReson"] = getResetReason();
    esp["freePsRam"] = ESP.getFreePsram();
    JsonObject spiffs = response.createNestedObject("spiffs");
    spiffs["totalBytes"] = SPIFFS.totalBytes();
    spiffs["usedBytes"] = SPIFFS.usedBytes();
    JsonObject camera = response.createNestedObject("camera");
    sensor_t *sensor = esp_camera_sensor_get();
    camera["framesize"] = sensor->status.framesize;
    camera["quality"] = sensor->status.quality;
    camera["errorCode"] = cameraErrorCode;
    #endif
    
    JsonObject status = response.createNestedObject("status");
    status["isSdAvailable"] = isStorageAvailable;
    status["isDisplayAvailable"] = isDisplayAvailable;

    char buffer[512];
    serializeJson(response, buffer);
    return String(buffer);
});