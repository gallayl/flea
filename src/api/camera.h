#pragma once

#include "../mime.h"
#include "../hw/Camera.h"

#define PART_BOUNDARY "123456789000000000000987654321"

static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "image/jpeg\r\nContent-Length: %u\r\n\r\n";

// ToDo: https://randomnerdtutorials.com/esp32-cam-video-streaming-web-server-camera-home-assistant/

ArRequestHandlerFunction getCameraStream = ([](AsyncWebServerRequest *request) {
    camera_fb_t *fb = NULL;
    char *part_buf[64];

    AsyncResponseStream *response = request->beginResponseStream(_STREAM_CONTENT_TYPE, fb->len);

    // while (true)
    // {
        fb = esp_camera_fb_get();
        if (!fb)
        {
            logInfo("Camera capture failed");
            return;
        }
        snprintf((char *)part_buf, 64, _STREAM_PART, fb->len);
        response->addHeader("Content-Type", (char *)part_buf);
        response->addHeader("boundary", _STREAM_BOUNDARY);
        response->write(fb->buf, fb->len);
        response->write(_STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        if (fb)
        {
            esp_camera_fb_return(fb);
            fb = NULL;
        }
    // }
    request->send(response);
});

ArRequestHandlerFunction getCameraImage = ([](AsyncWebServerRequest *request) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
        request->send(500, MIME_plainText, "Camera capture failed");
        esp_camera_fb_return(fb);
    }
    else
    {

        AsyncResponseStream *response = request->beginResponseStream(MIME_jpeg, fb->len);
        response->write(fb->buf, fb->len);
        request->send(response);
        esp_camera_fb_return(fb);
    }
});

ArRequestHandlerFunction setupCamera = ([](AsyncWebServerRequest *request) {
    StaticJsonDocument<512> response;
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
    request->send(200, MIME_json, String(buffer));
});