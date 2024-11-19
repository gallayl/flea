#pragma once

#include "../services/Logger.h"

ArRequestHandlerFunction listFiles = ([](AsyncWebServerRequest *request)
                                      {
                                          fs::File file = LittleFS.open("/", "r");

                                          if (!file || !file.isDirectory())
                                          {
                                              request->send(404, "");
                                          };

                                          JsonDocument response;
                                          JsonArray fileList = response.to<JsonArray>();

                                          file = file.openNextFile();

                                          while (file)
                                          {
                                            fileList.add(file.name());
                                            file = file.openNextFile();
                                          };

                                          String responseStr;
                                          serializeJson(response, responseStr);
                                          request->send(200, "application/json", responseStr); });
