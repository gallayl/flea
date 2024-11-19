#pragma once

#include "../services/Logger.h"

ArRequestHandlerFunction showLog = ([](AsyncWebServerRequest *request)
                                    {
                                          JsonDocument response = getLogResponse();
                                          String responseStr;
                                          serializeJson(response, responseStr);
                                          request->send(200, MIME_json, responseStr); });
