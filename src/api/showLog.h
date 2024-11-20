#pragma once

#include <ArduinoJson.h>
#include "../FeatureRegistry/Features/Logging.h"
#include "../mime.h"

ArRequestHandlerFunction showLog = ([](AsyncWebServerRequest *request)
                                    {
                                          JsonDocument response = LoggerInstance->getEntries();
                                          String responseStr;
                                          serializeJson(response, responseStr);
                                          request->send(200, MIME_json, responseStr); });
