#pragma once


#define ENABLE_I2C_DISPLAY false

#include "./Feature.h"
#include "./Features/Logging.h"
#include "./Features/SystemUtils.h"

#if ENABLE_I2C_DISPLAY
#include "./Features/Display.h"
#endif

#include "./Features/SerialRead.h"

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
        this->RegisterFeature(*SerialReadFeature);
    }


    void RegisterFeature(Feature newCommand)
    {
        this->RegisteredFeatures[this->_registeredFeaturesCount] = newCommand;
        this->_registeredFeaturesCount++;
    }

    /**
     * TODO: Check if its OK
     */
    JsonArray GetFeaturesArray()
    {
        JsonArray response = JsonArray();
        for (uint8_t i = 0; i < this->_registeredFeaturesCount; i++)
        {
            response.add(this->RegisteredFeatures[i].GetFeatureName());
        }

        return response;
    }

    void SetupFeatures()
    {
        for (uint8_t i = 0; i < this->_registeredFeaturesCount; i++)
        {
            LoggerInstance->Info("Setting up feature: " + this->RegisteredFeatures[i].GetFeatureName());
            this->RegisteredFeatures[i].Setup();
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
