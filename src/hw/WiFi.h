#pragma once

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <WiFiClientSecure.h>

#include "../FeatureRegistry/Features/Logging.h"

String getSignalStrength(int32_t rssi)
{
    if (rssi > -30)
    {
        return F("Amazing");
    }
    else if (rssi > -67)
    {
        return F("Very good");
    }
    else if (rssi > -70)
    {
        return F("Okay (not good, not terrible)");
    }
    else if (rssi > -80)
    {
        return F("Not good");
    }
    else if (rssi > -90)
    {
        return F("Unusable");
    }
    return F("Unknown");
}

#ifdef ESP32
String getEncryptionType(wifi_auth_mode_t type)
{
    switch (type)
    {
    case WIFI_AUTH_OPEN:
        return F("WIFI_AUTH_OPEN"); /**< authenticate mode : open */
    case WIFI_AUTH_WEP:
        return F("WIFI_AUTH_WEP"); /**< authenticate mode : WEP */
    case WIFI_AUTH_WPA_PSK:
        return F("WIFI_AUTH_WPA_PSK"); /**< authenticate mode : WPA_PSK */
    case WIFI_AUTH_WPA2_PSK:
        return F("WIFI_AUTH_WPA2_PSK"); /**< authenticate mode : WPA2_PSK */
    case WIFI_AUTH_WPA_WPA2_PSK:
        return F("WIFI_AUTH_WPA_WPA2_PSK"); /**< authenticate mode : WPA_WPA2_PSK */
    case WIFI_AUTH_WPA2_ENTERPRISE:
        return F("WIFI_AUTH_WPA2_ENTERPRISE"); /**< authenticate mode : WPA2_ENTERPRISE */
    case WIFI_AUTH_MAX:
        return F("WIFI_AUTH_MAX");
    default:
        return F("Unknown");
    }
}
#else
String getEncryptionType(uint8_t type)
{
    switch (type)
    {
    case ENC_TYPE_WEP:
        return F("ENC_TYPE_WEP");
    case ENC_TYPE_TKIP:
        return F("ENC_TYPE_TKIP");
    case ENC_TYPE_CCMP:
        return F("ENC_TYPE_CCMP");
    case ENC_TYPE_NONE:
        return F("ENC_TYPE_NONE");
    case ENC_TYPE_AUTO:
        return F("ENC_TYPE_AUTO");
    }
    return "Unknown";
}
#endif
#ifdef ESP32
void WiFiEvent(WiFiEvent_t event)
{

    switch (event)
    {
    case SYSTEM_EVENT_WIFI_READY:
        Logger::GetInstance()->Info(F("WiFi interface ready"));
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        Logger::GetInstance()->Info(F("Completed scan for access points"));
        break;
    case SYSTEM_EVENT_STA_START:
        Logger::GetInstance()->Info(F("WiFi client started"));
        break;
    case SYSTEM_EVENT_STA_STOP:
        Logger::GetInstance()->Info(F("WiFi clients stopped"));
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        Logger::GetInstance()->Info(F("Connected to access point"));
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Logger::GetInstance()->Info(F("Disconnected from WiFi access point"));
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        Logger::GetInstance()->Info(F("Authentication mode of access point has changed"));
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        Logger::GetInstance()->Info("Obtained IP address: " + WiFi.localIP().toString());
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        Logger::GetInstance()->Info(F("Lost IP address and IP address is reset to 0"));
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        Logger::GetInstance()->Info(F("WiFi Protected Setup (WPS): succeeded in enrollee mode"));
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        Logger::GetInstance()->Info(F("WiFi Protected Setup (WPS): failed in enrollee mode"));
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        Logger::GetInstance()->Info(F("WiFi Protected Setup (WPS): timeout in enrollee mode"));
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        Logger::GetInstance()->Info(F("WiFi Protected Setup (WPS): pin code in enrollee mode"));
        break;
    case SYSTEM_EVENT_AP_START:
        Logger::GetInstance()->Info(F("WiFi access point started"));
        break;
    case SYSTEM_EVENT_AP_STOP:
        Logger::GetInstance()->Info(F("WiFi access point  stopped"));
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        Logger::GetInstance()->Info(F("Client connected"));
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        Logger::GetInstance()->Info(F("Client disconnected"));
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        Logger::GetInstance()->Info(F("Assigned IP address to client"));
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        Logger::GetInstance()->Info(F("Received probe request"));
        break;
    case SYSTEM_EVENT_GOT_IP6:
        Logger::GetInstance()->Info(F("IPv6 is preferred"));
        break;
    case SYSTEM_EVENT_ETH_START:
        Logger::GetInstance()->Info(F("Ethernet started"));
        break;
    case SYSTEM_EVENT_ETH_STOP:
        Logger::GetInstance()->Info(F("Ethernet stopped"));
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        Logger::GetInstance()->Info(F("Ethernet connected"));
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        Logger::GetInstance()->Info(F("Ethernet disconnected"));
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        Logger::GetInstance()->Info(F("Obtained IP address"));
        break;
    default:
        break;
    }
}
#endif

void reinitWifiSettings()
{

#ifdef ESP32
    wifi_mode_t wifiMode = configJson[CONFIG_SOFT_AP_ENABLED].as<bool>() ? WIFI_AP_STA : WIFI_AP;
#else
    WiFiMode_t wifiMode = configJson[CONFIG_SOFT_AP_ENABLED].as<bool>() ? WIFI_AP_STA : WIFI_AP;
#endif

    if (wifiMode == WIFI_AP_STA)
    {
        IPAddress ip;
        IPAddress gateway;
        IPAddress netmask;

        ip.fromString(configJson[CONFIG_SOFT_AP_IP].as<String>());
        gateway.fromString(configJson[CONFIG_SOFT_AP_GATEWAY].as<String>());
        netmask.fromString(configJson[CONFIG_SOFT_AP_NETMASK].as<String>());

        WiFi.mode(wifiMode);
        WiFi.softAPConfig(ip, gateway, netmask);
        WiFi.softAP(configJson[CONFIG_SOFT_AP_SSID].as<String>().c_str(), configJson[CONFIG_SOFT_AP_KEY].as<String>().c_str());
    }
}

void initWifi()
{

    reinitWifiSettings();

#ifdef ESP32
    WiFi.onEvent(WiFiEvent);
#endif
    WiFi.begin();
}