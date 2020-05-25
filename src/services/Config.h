#pragma once

#ifdef ESP32
#include <SPIFFS.h>
#else
#include <FS.h>
#endif

#include "./Logger.h"
#include "../utils/JsonMerge.h"
#include "./TaskScheduler.h"

#define CONFIG_FILE "/config.json"
#define CONFIG_SIZE 2048
#define CONFIG_VALUE_SIZE 256

#define CONFIG_HTTP_PORT "httpPort"
#define CONFIG_WEBSOCKETS_URL "webSocketsUrl"
#define CONFIG_WEBSOCKETS_CLIENT_LIMIT "webSocketsClientLimit"

#define CONFIG_SOFT_AP_ENABLED "softApEnabled"
#define CONFIG_SOFT_AP_IP "softApIp"
#define CONFIG_SOFT_AP_GATEWAY "softApGateway"
#define CONFIG_SOFT_AP_NETMASK "softApNetmask"

#define CONFIG_SOFT_AP_SSID "softApSSID"
#define CONFIG_SOFT_AP_KEY "softApKey"

StaticJsonDocument<CONFIG_SIZE> configJson;



boolean isSaveNeeded = false;

void saveConfigCallback()
{
    if (isSaveNeeded)
    {
        #ifdef ESP32
        File configFile = SPIFFS.open(CONFIG_FILE, FILE_WRITE);
        #else
        File configFile = SPIFFS.open(CONFIG_FILE, "w");
        #endif
        if (!configFile)
        {
            logInfo("There was an error writing the config!");
        }
        char buffer[CONFIG_SIZE];
        serializeJsonPretty(configJson, buffer);
        configFile.print(buffer);
        configFile.close();
        logInfo("Config saved.");
        isSaveNeeded = false;
    }
}

void saveConfigFile(){
    isSaveNeeded = true;
}

void setDefaultConfig()
{
    configJson.clear();
    configJson[CONFIG_HTTP_PORT] = 80;
    configJson[CONFIG_WEBSOCKETS_URL] = "/ws";
    configJson[CONFIG_WEBSOCKETS_CLIENT_LIMIT] = 1;
    configJson[CONFIG_SOFT_AP_ENABLED] = true;
    configJson[CONFIG_SOFT_AP_IP] = "192.168.0.1";
    configJson[CONFIG_SOFT_AP_GATEWAY] = "192.168.0.1";
    configJson[CONFIG_SOFT_AP_NETMASK] = "255.255.255.0";
    configJson[CONFIG_SOFT_AP_SSID] = "flea";
    configJson[CONFIG_SOFT_AP_KEY] = "flea12345";
}

Task updateConfigIfNeeded(5000, 0, &saveConfigCallback);


void initConfig()
{
    logInfo("Initializing Config...");
    setDefaultConfig();
    if (!SPIFFS.begin())
    {
        logInfo("SPIFFS not available, config will be the default");
        return;
    }
    else
    {
        #ifdef ESP32
        File file = SPIFFS.open(CONFIG_FILE);
        #else
        File file = SPIFFS.open(CONFIG_FILE, "r");
        #endif
        if (file)
        {
            StaticJsonDocument<CONFIG_SIZE> fromFile;
            DeserializationError error = deserializeJson(fromFile, file.readString());
            switch (error.code())
            {
            case DeserializationError::Ok:
                logInfo(F("Deserialization succeeded"));
                merge(configJson.as<JsonVariant>(), fromFile.as<JsonVariantConst>());
                break;
            case DeserializationError::InvalidInput:
                logInfo(F("Invalid input!"));
                break;
            case DeserializationError::NoMemory:
                logInfo(F("Not enough memory"));
                break;
            default:
                logInfo(F("Deserialization failed"));
                break;
            }
        }
    }
    runner.addTask(updateConfigIfNeeded);
}


void setConfigValue(String key, String value)
{
    configJson[key] = value;
    saveConfigFile();
}

void setConfigValue(String key, int value)
{
    configJson[key] = value;
    saveConfigFile();
}

void setConfigValue(String key, float value)
{
    configJson[key] = value;
    saveConfigFile();
}
