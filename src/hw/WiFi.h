#pragma once

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <WiFiClientSecure.h>
#include "../config.h"
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
void WiFiEventHandlerESP32(WiFiEvent_t event)
{

    switch (event)
    {
    case SYSTEM_EVENT_WIFI_READY:
        LoggerInstance->Info(F("WiFi interface ready"));
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        LoggerInstance->Info(F("Completed scan for access points"));
        break;
    case SYSTEM_EVENT_STA_START:
        LoggerInstance->Info(F("WiFi client started"));
        break;
    case SYSTEM_EVENT_STA_STOP:
        LoggerInstance->Info(F("WiFi clients stopped"));
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        LoggerInstance->Info(F("Connected to access point"));
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        LoggerInstance->Info(F("Disconnected from WiFi access point"));
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        LoggerInstance->Info(F("Authentication mode of access point has changed"));
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        LoggerInstance->Info("Obtained IP address: " + WiFi.localIP().toString());
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        LoggerInstance->Info(F("Lost IP address and IP address is reset to 0"));
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        LoggerInstance->Info(F("WiFi Protected Setup (WPS): succeeded in enrollee mode"));
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        LoggerInstance->Info(F("WiFi Protected Setup (WPS): failed in enrollee mode"));
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        LoggerInstance->Info(F("WiFi Protected Setup (WPS): timeout in enrollee mode"));
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        LoggerInstance->Info(F("WiFi Protected Setup (WPS): pin code in enrollee mode"));
        break;
    case SYSTEM_EVENT_AP_START:
        LoggerInstance->Info(F("WiFi access point started"));
        break;
    case SYSTEM_EVENT_AP_STOP:
        LoggerInstance->Info(F("WiFi access point  stopped"));
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        LoggerInstance->Info(F("Client connected"));
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        LoggerInstance->Info(F("Client disconnected"));
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        LoggerInstance->Info(F("Assigned IP address to client"));
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        LoggerInstance->Info(F("Received probe request"));
        break;
    case SYSTEM_EVENT_GOT_IP6:
        LoggerInstance->Info(F("IPv6 is preferred"));
        break;
    case SYSTEM_EVENT_ETH_START:
        LoggerInstance->Info(F("Ethernet started"));
        break;
    case SYSTEM_EVENT_ETH_STOP:
        LoggerInstance->Info(F("Ethernet stopped"));
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        LoggerInstance->Info(F("Ethernet connected"));
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        LoggerInstance->Info(F("Ethernet disconnected"));
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        LoggerInstance->Info(F("Obtained IP address"));
        break;
    default:
        break;
    }
}
#endif

void startStaMode(String ssid, String staPassPharse)
{
    if (WiFi.getMode() == WIFI_AP &&  WiFi.begin() != WL_CONNECTED){
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP(ssid, staPassPharse);
    }
}

void initWifi()
{

#ifdef ESP32
    WiFi.onEvent(WiFiEventHandlerESP32);
#endif

#ifdef ESP8266

    WiFi.onStationModeConnected([](const WiFiEventStationModeConnected &event) {
        LoggerInstance->Info(F("Connected to access point"));
    });

    WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected &event) {
        LoggerInstance->Info(F("Disconnected from WiFi access point"));
    });

    WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP &event) {
        LoggerInstance->Info("Obtained IP address: " + WiFi.localIP().toString());
    });

    WiFi.onStationModeAuthModeChanged([](const WiFiEventStationModeAuthModeChanged &event) {
        LoggerInstance->Info(F("Authentication mode of access point has changed"));
    });

    WiFi.onSoftAPModeStationConnected([](const WiFiEventSoftAPModeStationConnected &event) {
        LoggerInstance->Info(F("Client connected"));
    });

    WiFi.onSoftAPModeStationDisconnected([](const WiFiEventSoftAPModeStationDisconnected &event) {
        LoggerInstance->Info(F("Client disconnected"));
    });

    WiFi.onSoftAPModeProbeRequestReceived([](const WiFiEventSoftAPModeProbeRequestReceived &event) {
        LoggerInstance->Info(F("Received probe request"));
    });

    WiFi.onWiFiModeChange([](const WiFiEventModeChange &event) {
        LoggerInstance->Info(F("WiFi mode changed"));
    });
#endif

    WiFi.mode(WIFI_AP);
    WiFi.begin();
    wl_status_t state = (wl_status_t)WiFi.waitForConnectResult();

    if (state != WL_CONNECTED)
    {
        LoggerInstance->Error(F("Failed to connect to access point"));
        startStaMode(STA_SSID, STA_PASSPHARSE);
    }
    else
    {
        LoggerInstance->Info(F("Connected to access point"));
    }
}