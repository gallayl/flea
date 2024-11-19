#pragma once

ArRequestHandlerFunction listFiles = ([](AsyncWebServerRequest *request)
                                      {
                                          fs::File root = LittleFS.open("/", "r");

                                          if (!root || !root.isDirectory())
                                          {
                                              request->send(404, "");
                                          };

                                          JsonDocument response;
                                          JsonArray fileList = response.to<JsonArray>();

#ifdef ESP32
                                          File file = root.openNextFile("r");
#else
                                          File file = root.openNextFile();
#endif
                                          do
                                          {
                                            JsonObject o = fileList.add<JsonObject>();
                                            o["name"] = file.name();
                                            o["size"] = file.size();
                                            o["isDir"] = file.isDirectory();
#ifdef ESP32
                                            o["path"] = file.path();
#endif
                                            o["lastWrite"] = file.getLastWrite();

#ifdef ESP32
                                          file = root.openNextFile("r");
#else
                                          file = root.openNextFile();
#endif


                                          } while (file);

                                          String responseStr;
                                          serializeJson(response, responseStr);
                                          request->send(200, MIME_json, responseStr); });
