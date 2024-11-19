#pragma once

#include <ArduinoJson.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <IPAddress.h>

#include "../../hw/WiFi.h"

#include "../CommandParser.h"
#include "../CustomCommand.h"

CustomCommand *wifiCommand = new CustomCommand("wifi", [](String command)
                                               {
    String operation = CommandParser::GetCommandParameter(command, 1);
    if (!operation.compareTo("connect"))
    {
        String ssid = CommandParser::GetCommandParameter(command, 2);
        String password = CommandParser::GetCommandParameter(command, 3);
        if (ssid.length() < 3 || password.length() < 5)
        {
            return String("{\"error\": \"ssid or password too short\"}");
        }
        WiFi.persistent(true);
        WiFi.begin(ssid.c_str(), password.c_str());
        return String("{\"event\": \"connecting\"}");
    }
    if (!operation.compareTo("list"))
    {
        JsonDocument response;
        JsonArray networks = response["networks"].as<JsonArray>();

        int n = WiFi.scanNetworks();
        networks.begin();

        for (int i = 0; i < n; ++i)
        {
            JsonObject element;
            element["ssid"] = WiFi.SSID(i);
            element["rssi"] = WiFi.RSSI(i);
            element["rssiText"] = getSignalStrength(WiFi.RSSI(i));
            element["encryption"] = getEncryptionType(WiFi.encryptionType(i));
            networks.add(element);
        }
        networks.end();
        char buffer[512];
        serializeJson(response, buffer);
        return String(buffer);
    }
    if (!operation.compareTo("startSTA"))
    {
        String ssid = CommandParser::GetCommandParameter(command, 2);
        String passpharse = CommandParser::GetCommandParameter(command, 3);
        if (ssid.length() < 3 || passpharse.length() < 5)
        {
            return String("{\"error\": \"ssid or passpharse too short\"}");
        }

        setConfigValue(CONFIG_SOFT_AP_SSID, ssid);
        setConfigValue(CONFIG_SOFT_AP_KEY, passpharse);
        setConfigValue(CONFIG_SOFT_AP_ENABLED, true);
        reinitWifiSettings();
        return String("{\"event\": \"starting STA\"}");
    }

    if (!operation.compareTo("stopSTA"))
    {
        bool success = WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_AP);
        return String("{\"event\": \"stopSTA\", \"success\": " + String(success) + "}");
    }

    if (!operation.compareTo("info"))
    {
        JsonDocument response;
        if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA)
        {
            JsonObject ap = response["ap"].to<JsonObject>();
            ap["ipAddress"] = WiFi.localIP().toString();
            ap["macAddress"] = WiFi.macAddress();
            ap["ssid"] = WiFi.SSID();
        }


        if (WiFi.getMode() == WIFI_STA || WiFi.getMode() == WIFI_AP_STA)
        {
            JsonObject sta = response["sta"].to<JsonObject>();
            sta["ipAddress"] = WiFi.softAPIP().toString();
            sta["gateway"] = configJson[CONFIG_SOFT_AP_GATEWAY];
            sta["netmask"] = configJson[CONFIG_SOFT_AP_NETMASK];
            sta["macAddress"] = WiFi.softAPmacAddress();
            sta["ssid"] = configJson[CONFIG_SOFT_AP_SSID];

        }

        int32_t rssi = WiFi.RSSI();
        response["wifiStrengh"] = getSignalStrength(rssi);
        response["wifiRssiDb"] = rssi;

        char buffer[512];
        serializeJson(response, buffer);
        return String(buffer);
    }
    return String("{\"event\": \"Unknown WiFi operation command. The available commands are: info, list, connect <ssid> <password>, startSTA<ssid, password>, stopSTA\"}"); });