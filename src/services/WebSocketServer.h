#pragma once

#include <ESPAsyncWebServer.h>

#ifdef ESP32
#include <Update.h>
#else
#include <ArduinoOTA.h>
#endif

#include "./Config.h"
#include "./WebServer.h"
#include "../CommandInterpreter/CommandInterpreter.h"

AsyncWebSocket *webSocket;

void cleanupSocketsCallback()
{
    webSocket->cleanupClients(configJson[CONFIG_WEBSOCKETS_CLIENT_LIMIT].as<int>());
}

Task cleanupUnusedSockets(5000, 1, &cleanupSocketsCallback);

void initWebSockets()
{
    webSocket = new AsyncWebSocket(configJson[CONFIG_WEBSOCKETS_URL].as<String>());

    webSocket->onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
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
            String response = CommandInterpreter::GetInstance()->ExecuteCommand(str);
            server->textAll(response);
        }
    });

    server->addHandler(webSocket);
}