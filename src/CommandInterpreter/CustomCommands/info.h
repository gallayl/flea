#pragma once

#include "../CommandParser.h"
#include "../CustomCommand.h"
#include "../../hw/Camera.h"
#include "../../hw/Display.h"
#include "../../services/FtpServer.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <SPIFFS.h>

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

String getSignalStrength(int32_t rssi)
{
    if (rssi > -30)
    {
        return "Amazing";
    }
    else if (rssi > -67)
    {
        return "Very good";
    }
    else if (rssi > -70)
    {
        return "Okay (not good, not terrible)";
    }
    else if (rssi > -80)
    {
        return "Not good";
    }
    else if (rssi > -90)
    {
        return "Unusable";
    }
    return "Unknown";
}

CustomCommand *infoAction = new CustomCommand("info", [](String command) {
    StaticJsonDocument<512> response;

    JsonObject esp = response.createNestedObject("esp");

    esp["restartReson"] = getResetReason();
    esp["sdkVersion"] = ESP.getSdkVersion();
    esp["cpuFreqMhz"] = ESP.getCpuFreqMHz();
    esp["freeHeap"] = ESP.getFreeHeap();
    esp["freeSkSpace"] = ESP.getFreeSketchSpace();
    esp["freePsRam"] = ESP.getFreePsram();

    JsonObject flash = response.createNestedObject("flash");

    flash["mode"] = ESP.getFlashChipMode();
    flash["size"] = ESP.getFlashChipSize();
    flash["speed"] = ESP.getFlashChipSpeed();

    JsonObject spiffs = response.createNestedObject("spiffs");
    spiffs["totalBytes"] = SPIFFS.totalBytes();
    spiffs["usedBytes"] = SPIFFS.usedBytes();
    
    JsonObject network = response.createNestedObject("network");
    network["ipAddress"] = WiFi.localIP().toString();
    network["macAddress"] = WiFi.macAddress();
    int32_t rssi = WiFi.RSSI();
    network["ssid"] = WiFi.SSID();
    network["wifiStrengh"] = getSignalStrength(rssi);
    network["wifiRssiDb"] = rssi;

    JsonObject status = response.createNestedObject("status");
    status["isSdAvailable"] = isStorageAvailable;
    status["isDisplayAvailable"] = isDisplayAvailable;

    JsonObject camera = response.createNestedObject("camera");

    sensor_t *sensor = esp_camera_sensor_get();
   
    camera["framesize"] = sensor->status.framesize;
    camera["quality"] = sensor->status.quality;
    camera["errorCode"] = cameraErrorCode;

    char buffer[512];
    serializeJson(response, buffer);
    return String(buffer);
});