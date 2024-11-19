#pragma once

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "./Config.h"
#include "./Logger.h"
#include "../mime.h"
#include "../api/update.h"
#include "../api/camera.h"
#include "../api/lights.h"
#include "../api/upload.h"
#include "../api/list.h"
#include "../api/showLog.h"

AsyncWebServer *server;

void initWebServer()
{
    uint8_t port = configJson[CONFIG_HTTP_PORT].as<int>();

    logInfo(F("Starting WEB server on port: ") + String(port));

    server = new AsyncWebServer(port);

    // Simple Firmware Update Form
    server->on("/update", HTTP_GET, getUpdateForm);
    server->on("/update", HTTP_POST, onPostUpdate, onUploadUpdate);

    server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, MIME_plainText, String(ESP.getFreeHeap())); });

#ifdef ESP32
    server->on("/cam", HTTP_GET, getCameraImage);
    server->on("/stream", HTTP_GET, getCameraStream);
    server->on("/setupCam", HTTP_GET, setupCamera);
    server->on("/lights", HTTP_GET, setLights);
#endif

    server->on("/uploadFiles", HTTP_POST, onPostUploadFiles, uploadFiles);
    server->on("/listFiles", HTTP_GET, listFiles);

    server->on("/showLog", HTTP_GET, showLog);

    server->serveStatic("/", LittleFS, "/", "max-age=600").setDefaultFile("index.html");

    server->onNotFound([](AsyncWebServerRequest *req)
                       {
                        logInfo("Not found: " + req->url());
        AsyncWebServerResponse *response = req->beginResponse(LittleFS, "/index.html", "text/html; charset=UTF-8");
        req->send(response); });

    server->begin();

    logInfo(F("Server setup done."));
}