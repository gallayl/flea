#pragma once

#include "../config.h"
#include "./registeredFeatures.h"
#include "./Feature.h"
#include "./Features/Logging.h"
#include "./Features/SystemFeatures.h"

#if ENABLE_I2C_DISPLAY
#include "./Features/Display.h"
#endif

#if ENABLE_SERIAL_READ
#include "./Features/SerialRead.h"
#endif

#if ENABLE_PWM
#include "./Features/Pwm/PwmFeature.h"
#endif

#if ENABLE_LITTLEFS
#include "./Features/LittleFsManagement/LittleFsManagement.h"
#endif

#ifdef ESP32

#if ENABLE_CAMERA
#include "./Features/esp32/Camera.h"
#endif

#endif



#define FEATURES_SIZE 128


class FeatureRegistry
{
private:

    String _featureNames[FEATURES_SIZE];

    uint8_t _registeredFeaturesCount = 0;


public:

    FeatureRegistry()
    {
        this->RegisterFeature(*LoggingFeature);
        this->RegisterFeature(*SystemFeatures);
#if ENABLE_I2C_DISPLAY
        this->RegisterFeature(*DisplayFeature);
#endif
#if ENABLE_SERIAL_READ
        this->RegisterFeature(*SerialReadFeature);
#endif

#if ENABLE_PWM
        this->RegisterFeature(*PwmFeature);
#endif

#if ENABLE_LITTLEFS
        this->RegisterFeature(*LittleFsFeature);
#endif

    }


    void RegisterFeature(Feature newFeature)
    {
        this->RegisteredFeatures[this->_registeredFeaturesCount] = newFeature;
        this->_registeredFeaturesCount++;
        String featureName = newFeature.GetFeatureName();

        JsonObject featureEntry = registeredFeatures[featureName].to<JsonObject>();
        featureEntry["name"] = featureName;
        featureEntry["state"] = (int) newFeature.GetFeatureState();
    }

    void SetupFeatures()
    {
        for (uint8_t i = 0; i < this->_registeredFeaturesCount; i++)
        {
            String featureName = this->RegisteredFeatures[i].GetFeatureName();
            LoggerInstance->Info("Setting up feature: " + featureName);
            FeatureState newState = this->RegisteredFeatures[i].Setup();
            JsonObject feature = registeredFeatures[featureName];
            feature["state"].set((int) newState);
        }
    }

    void LoopFeatures()
    {
        for (uint8_t i = 0; i < this->_registeredFeaturesCount; i++)
        {
            this->RegisteredFeatures[i].Loop();
        }
    }

    Feature RegisteredFeatures[FEATURES_SIZE];

};

FeatureRegistry *FeatureRegistryInstance = new FeatureRegistry();
