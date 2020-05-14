#pragma once

#include <ESPAsyncWebServer.h>
#include "CommandInterpreter/CommandInterpreter.h"
#include "./services/Logger.h"

#include <SPIFFS.h>

#include "./mime.h"
#include "./api/update.h"
#include "./api/camera.h"
#include "./api/lights.h"

class McuServer
{
public:
    McuServer(char *user, char *password, CommandInterpreter *commandInterpreter) : webSocket(*(new AsyncWebSocket("/ws"))), webServer(*(new AsyncWebServer(80))), commandInterpreter(commandInterpreter)
    {
        this->wwwUserName = user;
        this->wwwPassword = password;
    };

    void setup()
    {
        logInfo(F("Starting WEB server..."));

        this->webSocket.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
            if (type == WS_EVT_CONNECT)
            {
                client->text("Connected to Rover ESP32");
                logInfo(String("WS connected: " + client->remoteIP().toString()));
            }
            else if (type == WS_EVT_DISCONNECT)
            {
                logInfo(String("WS left: " + client->remoteIP().toString()));
            }
            else if (type == WS_EVT_DATA)
            {
                String str = String((char *)data).substring(0, len);
                String response = this->commandInterpreter->ExecuteCommand(str);
                server->textAll(response);
            }
        });

        this->webServer.addHandler(&(this->webSocket));

        this->webServer.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

        // Simple Firmware Update Form
        this->webServer.on("/update", HTTP_GET, getUpdateForm);
        this->webServer.on("/update", HTTP_POST, onPostUpdate, onUploadUpdate);

        this->webServer.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(200, MIME_plainText, String(ESP.getFreeHeap()));
        });

        this->webServer.on("/cam", HTTP_GET, getCameraImage);
        this->webServer.on("/stream", HTTP_GET, getCameraStream);
        
        this->webServer.on("/setupCam", HTTP_GET, setupCamera);
        this->webServer.on("/lights", HTTP_GET, setLights);

        this->webServer.onNotFound([](AsyncWebServerRequest *req) {
            req->send(404, MIME_plainText, "Not found :(");
        });

        this->webServer.begin();
        
        logInfo(F("Server setup done."));
    }

    void closeSockets()
    {
        this->webSocket.textAll("Closing connection...");
        this->webSocket.closeAll();
    }

    void broadcast(String text)
    {
        this->webSocket.textAll(text);
    }

private:
    const char *wwwUserName;
    const char *wwwPassword;

    AsyncWebSocket &webSocket;
    AsyncWebServer &webServer;
    CommandInterpreter *commandInterpreter;
};
