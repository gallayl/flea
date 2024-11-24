#pragma once

#include <LittleFS.h>

#include "./Logger.h"
#include "../utils/JsonMerge.h"

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

JsonDocument configJson;

void saveConfigFile()
{
    File configFile = LittleFS.open(CONFIG_FILE, "w");
    if (!configFile)
    {
        logInfo("There was an error writing the config!");
    }
    serializeJsonPretty(configJson, configFile);
    configFile.close();
    logInfo("Config saved.");
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

void initConfig()
{
    logInfo("Initializing Config...");
    setDefaultConfig();

#ifdef ESP32
    if (!LittleFS.begin(true))
#else
    if (!LittleFS.begin())
#endif
    {
        logInfo("LittleFS not available, config will be the default");
        return;
    }
    else
    {
        File file = LittleFS.open(CONFIG_FILE, "r");
        if (file)
        {
            JsonDocument fromFile;
            DeserializationError error = deserializeJson(fromFile, file);
            switch (error.code())
            {
            case DeserializationError::Ok:
                logInfo(F("Config file loaded succesfully."));
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
        else
        {
            logInfo("No config file, using defaults.");
        }
    }
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
