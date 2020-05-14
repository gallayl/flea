#pragma once

#include <SPIFFS.h>
#include "./Logger.h"
#include "../utils/JsonMerge.h"

#define CONFIG_FILE "/config.json"
#define CONFIG_SIZE 2048
#define CONFIG_VALUE_SIZE 256

#define CONFIG_WIFI_MODE "configWifiMode"
#define CONFIG_WIFI_AP

StaticJsonDocument<CONFIG_SIZE> configJson;

void setDefaultConfig()
{
    configJson.clear();
}

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
        File file = SPIFFS.open(CONFIG_FILE);
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
}


void saveConfigFile(){
    File configFile = SPIFFS.open(CONFIG_FILE, FILE_WRITE);
    if (!configFile)
    {
        logInfo("There was an error writing the config!");
    }
    char buffer[CONFIG_SIZE];
    serializeJsonPretty(configJson, buffer);
    configFile.print(buffer);
    configFile.close();
    logInfo("Config saved.");
}

void setConfigValue(String key, String value)
{
    configJson[key] = value;
    saveConfigFile();
}