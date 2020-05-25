#pragma once

#include <ESPAsyncWebServer.h>
#ifdef ESP32
#include <SPIFFS.h>
#else 
#include <FS.h>
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

    server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

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
    server->onNotFound([](AsyncWebServerRequest *req) {
        req->send(404, MIME_plainText, "Not found :(");
    });

    server->begin();

    logInfo(F("Server setup done."));
}