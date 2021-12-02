#pragma once

#include <ESPAsyncWebServer.h>
#ifdef ESP32
#include <LITTLEFS.h>
#else
#include <LittleFS.h>
#endif
#include "./Config.h"
#include "./Logger.h"
#include "../mime.h"
#include "../api/update.h"
#include "../api/camera.h"
#include "../api/lights.h"

AsyncWebServer *server;

void initWebServer()
{
    logInfo(F("Starting WEB server..."));
    uint8_t port = configJson[CONFIG_HTTP_PORT].as<int>();
    server = new AsyncWebServer(port);

    // Simple Firmware Update Form
    server->on("/update", HTTP_GET, getUpdateForm);
    server->on("/update", HTTP_POST, onPostUpdate, onUploadUpdate);

    server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, MIME_plainText, String(ESP.getFreeHeap()));
    });

#ifdef ESP32
    server->on("/cam", HTTP_GET, getCameraImage);
    server->on("/stream", HTTP_GET, getCameraStream);
    server->on("/setupCam", HTTP_GET, setupCamera);
    server->on("/lights", HTTP_GET, setLights);
#endif
    server->serveStatic("/", LittleFS, "/", "max-age=600").setDefaultFile("index.html");

    server->onNotFound([](AsyncWebServerRequest *req) {
        AsyncWebServerResponse *response = req->beginResponse(LittleFS, "/index.html", "text/html; charset=UTF-8");
        req->send(response);
    });
    server->begin();

    logInfo(F("Server setup done."));
}