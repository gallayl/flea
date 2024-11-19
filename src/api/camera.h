#pragma once

#ifdef ESP32

#include "../mime.h"
#include "../hw/Camera.h"
#include <esp32cam-asyncweb.h>
#include <ESPAsyncWebServer.h>

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

ArRequestHandlerFunction setupCamera = ([](AsyncWebServerRequest *request)
                                        {
    JsonDocument response;
    sensor_t *sensor = esp_camera_sensor_get();
    int res = 0;

    if (!sensor)
    {
        request->send(500, MIME_json, "Failed to get Sensor");
        return;
    }

    for (size_t i = 0; i < request->params(); i++)
    {
        AsyncWebParameter *param = request->getParam(i);
        String variable = param->name();
        String value = param->value();
        int val = value.toInt();

        if (!variable.compareTo("framesize"))
            res = sensor->set_framesize(sensor, (framesize_t)val);
        else if (!variable.compareTo("quality"))
            res = sensor->set_quality(sensor, val);
        else if (!variable.compareTo("contrast"))
            res = sensor->set_contrast(sensor, val);
        else if (!variable.compareTo("brightness"))
            res = sensor->set_brightness(sensor, val);
        else if (!variable.compareTo("saturation"))
            res = sensor->set_saturation(sensor, val);
        else if (!variable.compareTo("gainceiling"))
            res = sensor->set_gainceiling(sensor, (gainceiling_t)val);
        else if (!variable.compareTo("colorbar"))
            res = sensor->set_colorbar(sensor, val);
        else if (!variable.compareTo("awb"))
            res = sensor->set_whitebal(sensor, val);
        else if (!variable.compareTo("agc"))
            res = sensor->set_gain_ctrl(sensor, val);
        else if (!variable.compareTo("aec"))
            res = sensor->set_exposure_ctrl(sensor, val);
        else if (!variable.compareTo("hmirror"))
            res = sensor->set_hmirror(sensor, val);
        else if (!variable.compareTo("vflip"))
            res = sensor->set_vflip(sensor, val);
        else if (!variable.compareTo("awb_gain"))
            res = sensor->set_awb_gain(sensor, val);
        else if (!variable.compareTo("agc_gain"))
            res = sensor->set_agc_gain(sensor, val);
        else if (!variable.compareTo("aec_value"))
            res = sensor->set_aec_value(sensor, val);
        else if (!variable.compareTo("aec2"))
            res = sensor->set_aec2(sensor, val);
        else if (!variable.compareTo("dcw"))
            res = sensor->set_dcw(sensor, val);
        else if (!variable.compareTo("bpc"))
            res = sensor->set_bpc(sensor, val);
        else if (!variable.compareTo("wpc"))
            res = sensor->set_wpc(sensor, val);
        else if (!variable.compareTo("raw_gma"))
            res = sensor->set_raw_gma(sensor, val);
        else if (!variable.compareTo("lenc"))
            res = sensor->set_lenc(sensor, val);
        else if (!variable.compareTo("special_effect"))
            res = sensor->set_special_effect(sensor, val);
        else if (!variable.compareTo("wb_mode"))
            res = sensor->set_wb_mode(sensor, val);
        else if (!variable.compareTo("ae_level"))
            res = sensor->set_ae_level(sensor, val);
        JsonObject obj = response.createNestedObject(variable);
        obj["value"] = val;
        obj["response"] = res;
    }
    char buffer[512];
    serializeJson(response, buffer);
    request->send(200, MIME_json, String(buffer)); });

#endif