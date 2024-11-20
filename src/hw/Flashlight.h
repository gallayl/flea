#pragma once

#include "../FeatureRegistry/Features/Logging.h"

#define FLASHLIGHT_MAX_PWM_FREQ 255
#define FLASH_PIN 4
#define FLASH_PWM_CHANNEL 0

#ifdef ESP32

void initFlashlight() {
    Logger::GetInstance()->Info(F("Initializing Flashlight..."));
    ledcSetup(FLASH_PWM_CHANNEL,5000,8);
    ledcAttachPin(FLASH_PIN, FLASH_PWM_CHANNEL);
}

void flashlightOff() {
    ledcWrite(FLASH_PWM_CHANNEL, 0);
}

void flashlightOn() {
    ledcWrite(FLASH_PWM_CHANNEL, FLASHLIGHT_MAX_PWM_FREQ);
}

void setFlashlightDuty(uint32_t duty){
    ledcWrite( FLASH_PWM_CHANNEL, duty);
}

#endif