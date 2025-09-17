#pragma once

#ifdef ESP32

#include <ArduinoJson.h>
#include "../../../mime.h"
#include "../../../services/WebServer.h"
#include <esp32cam-asyncweb.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>
#define PART_BOUNDARY "frame"

static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

// ToDo: https://randomnerdtutorials.com/esp32-cam-video-streaming-web-server-camera-home-assistant/

/**
 * Starts a camera stream in MJPEG format
 */
ArRequestHandlerFunction getCameraStream = esp32cam::asyncweb::handleMjpeg;

ArRequestHandlerFunction getCameraImage = esp32cam::asyncweb::handleStill;


ArRequestHandlerFunction getCameraSettings = ([](AsyncWebServerRequest *request)
                                        {
    JsonDocument configuration;

    sensor_t *sensor = esp_camera_sensor_get();

    if (!sensor)
    {
        request->send(500, MIME_json, "Failed to get Sensor");
        return;
    }

    configuration["framesize"] = sensor->status.framesize;
    configuration["quality"] = sensor->status.quality;
    configuration["brightness"] = sensor->status.brightness;
    configuration["contrast"] = sensor->status.contrast;
    configuration["saturation"] = sensor->status.saturation;
    configuration["gainceiling"] = sensor->status.gainceiling;
    configuration["colorbar"] = sensor->status.colorbar;
    configuration["awb"] = sensor->status.awb;
    configuration["agc"] = sensor->status.agc;
    configuration["aec"] = sensor->status.aec;
    configuration["hmirror"] = sensor->status.hmirror;
    configuration["vflip"] = sensor->status.vflip;
    configuration["awb_gain"] = sensor->status.awb_gain;
    configuration["agc_gain"] = sensor->status.agc_gain;
    configuration["aec_value"] = sensor->status.aec_value;
    configuration["aec2"] = sensor->status.aec2;
    configuration["dcw"] = sensor->status.dcw;
    configuration["bpc"] = sensor->status.bpc;
    configuration["wpc"] = sensor->status.wpc;
    configuration["raw_gma"] = sensor->status.raw_gma;
    configuration["lenc"] = sensor->status.lenc;
    configuration["special_effect"] = sensor->status.special_effect;
    configuration["wb_mode"] = sensor->status.wb_mode;
    configuration["ae_level"] = sensor->status.ae_level;

    char buffer[8192];

    serializeJson(configuration, buffer);
    request->send(200, MIME_json, String(buffer)); });



ArBodyHandlerFunction updateCameraSettings = ([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                                            {

    JsonDocument doc;
    DeserializationError deserializationError = deserializeJson(doc, (char*)data);
    if (deserializationError)
    {
        request->send(400, MIME_json, deserializationError.f_str());
        return;
    }

    sensor_t *sensor = esp_camera_sensor_get();
    int res = 0;

    if (!sensor)
    {
        request->send(500, MIME_json, "Failed to get Sensor");
        return;
    }

    if (doc["framesize"].is<int>())
        res = sensor->set_framesize(sensor, (framesize_t)doc["framesize"].as<int>());
    if (doc["quality"].is<int>())
        res = sensor->set_quality(sensor, doc["quality"].as<int>());
    if (doc["contrast"].is<int>())    
        res = sensor->set_contrast(sensor, doc["contrast"].as<int>());
    if (doc["brightness"].is<int>())
        res = sensor->set_brightness(sensor, doc["brightness"].as<int>());
    if (doc["saturation"].is<int>())
        res = sensor->set_saturation(sensor, doc["saturation"].as<int>());
    if (doc["gainceiling"].is<int>())
        res = sensor->set_gainceiling(sensor, (gainceiling_t)doc["gainceiling"].as<int>());
    if (doc["colorbar"].is<int>())
        res = sensor->set_colorbar(sensor, doc["colorbar"].as<int>());
    if (doc["awb"].is<int>())
        res = sensor->set_whitebal(sensor, doc["awb"].as<int>());
    if (doc["agc"].is<int>())
        res = sensor->set_gain_ctrl(sensor, doc["agc"].as<int>());
    if (doc["aec"].is<int>())
        res = sensor->set_exposure_ctrl(sensor, doc["aec"].as<int>());
    if (doc["hmirror"].is<int>())
        res = sensor->set_hmirror(sensor, doc["hmirror"].as<int>());
    if (doc["vflip"].is<int>())
        res = sensor->set_vflip(sensor, doc["vflip"].as<int>());
    if (doc["awb_gain"].is<int>())
        res = sensor->set_awb_gain(sensor, doc["awb_gain"].as<int>());
    if (doc["agc_gain"].is<int>())
        res = sensor->set_agc_gain(sensor, doc["agc_gain"].as<int>());
    if (doc["aec_value"].is<int>())
        res = sensor->set_aec_value(sensor, doc["aec_value"].as<int>());
    if (doc["aec2"].is<int>())
        res = sensor->set_aec2(sensor, doc["aec2"].as<int>());
    if (doc["dcw"].is<int>())
        res = sensor->set_dcw(sensor, doc["dcw"].as<int>());
    if (doc["bpc"].is<int>())
        res = sensor->set_bpc(sensor, doc["bpc"].as<int>());
    if (doc["wpc"].is<int>())
        res = sensor->set_wpc(sensor, doc["wpc"].as<int>());
    if (doc["raw_gma"].is<int>())
        res = sensor->set_raw_gma(sensor, doc["raw_gma"].as<int>());
    if (doc["lenc"].is<int>()) 
        res = sensor->set_lenc(sensor, doc["lenc"].as<int>());
    if (doc["special_effect"].is<int>())
        res = sensor->set_special_effect(sensor, doc["special_effect"].as<int>());
    if (doc["wb_mode"].is<int>())
        res = sensor->set_wb_mode(sensor, doc["wb_mode"].as<int>());
    if (doc["ae_level"].is<int>())
       res = sensor->set_ae_level(sensor, doc["ae_level"].as<int>());

    // for (size_t i = 0; i < request->params(); i++)
    // {
    //     AsyncWebParameter *param = request->getParam(i);
    //     String variable = param->name();
    //     String value = param->value();
    //     int val = value.toInt();

    //     if (!variable.compareTo("framesize"))
    //         res = sensor->set_framesize(sensor, (framesize_t)val);
    //     else if (!variable.compareTo("quality"))
    //         res = sensor->set_quality(sensor, val);
    //     else if (!variable.compareTo("contrast"))
    //         res = sensor->set_contrast(sensor, val);
    //     else if (!variable.compareTo("brightness"))
    //         res = sensor->set_brightness(sensor, val);
    //     else if (!variable.compareTo("saturation"))
    //         res = sensor->set_saturation(sensor, val);
    //     else if (!variable.compareTo("gainceiling"))
    //         res = sensor->set_gainceiling(sensor, (gainceiling_t)val);
    //     else if (!variable.compareTo("colorbar"))
    //         res = sensor->set_colorbar(sensor, val);
    //     else if (!variable.compareTo("awb"))
    //         res = sensor->set_whitebal(sensor, val);
    //     else if (!variable.compareTo("agc"))
    //         res = sensor->set_gain_ctrl(sensor, val);
    //     else if (!variable.compareTo("aec"))
    //         res = sensor->set_exposure_ctrl(sensor, val);
    //     else if (!variable.compareTo("hmirror"))
    //         res = sensor->set_hmirror(sensor, val);
    //     else if (!variable.compareTo("vflip"))
    //         res = sensor->set_vflip(sensor, val);
    //     else if (!variable.compareTo("awb_gain"))
    //         res = sensor->set_awb_gain(sensor, val);
    //     else if (!variable.compareTo("agc_gain"))
    //         res = sensor->set_agc_gain(sensor, val);
    //     else if (!variable.compareTo("aec_value"))
    //         res = sensor->set_aec_value(sensor, val);
    //     else if (!variable.compareTo("aec2"))
    //         res = sensor->set_aec2(sensor, val);
    //     else if (!variable.compareTo("dcw"))
    //         res = sensor->set_dcw(sensor, val);
    //     else if (!variable.compareTo("bpc"))
    //         res = sensor->set_bpc(sensor, val);
    //     else if (!variable.compareTo("wpc"))
    //         res = sensor->set_wpc(sensor, val);
    //     else if (!variable.compareTo("raw_gma"))
    //         res = sensor->set_raw_gma(sensor, val);
    //     else if (!variable.compareTo("lenc"))
    //         res = sensor->set_lenc(sensor, val);
    //     else if (!variable.compareTo("special_effect"))
    //         res = sensor->set_special_effect(sensor, val);
    //     else if (!variable.compareTo("wb_mode"))
    //         res = sensor->set_wb_mode(sensor, val);
    //     else if (!variable.compareTo("ae_level"))
    //        res = sensor->set_ae_level(sensor, val);
    // }


    getCameraSettings(request); });


#endif
