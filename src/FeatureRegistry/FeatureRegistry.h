#pragma once
#include "./Feature.h"

#include "./Features/Logging.h"
#include "./Features/SerialRead.h"

#define FEATURES_SIZE 128

class FeatureRegistry
{
private:
    static FeatureRegistry *instance;

    String _featureNames[FEATURES_SIZE];

    uint8_t _registeredFeaturesCount = 0;
    void RegisterFeature(Feature newCommand)
    {
        this->RegisteredFeatures[this->_registeredFeaturesCount] = newCommand;
        this->_registeredFeaturesCount++;

        Logger::GetInstance()->Info("Registered feature: " + newCommand.GetFeatureName());
    }
    FeatureRegistry()
    {
    }

public:
    /**
     * TODO: Check if its OK
     */
    String *GetFeatureNames()
    {
        return this->_featureNames;
    }

    Feature RegisteredFeatures[FEATURES_SIZE];

    static FeatureRegistry *GetInstance()
    {
        if (instance == 0)
        {
            FeatureRegistry *createdInstance = new FeatureRegistry();
            createdInstance->RegisterFeature(*Logging);
            createdInstance->RegisterFeature(*SerialRead);

            instance = createdInstance;
        }

        return instance;
    }
};

FeatureRegistry *FeatureRegistry::instance = 0;
