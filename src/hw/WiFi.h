#pragma once

#include <WiFi.h>
#include <WiFiClientSecure.h>

const char *ssid = "Draconian";
const char *password = "Eszti65535";

void initWifi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
    }
}