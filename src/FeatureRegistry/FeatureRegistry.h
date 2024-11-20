#pragma once
#include "./Feature.h"

#include "./Features/Logging.h"
#include "./Features/Display.h"
#include "./Features/SerialRead.h"

#define FEATURES_SIZE 128

class FeatureRegistry
{
private:
    static FeatureRegistry *instance;

    String _featureNames[FEATURES_SIZE];

    uint8_t _registeredFeaturesCount = 0;

    FeatureRegistry()
    {
    }

public:

    void RegisterFeature(Feature newCommand)
    {
        this->RegisteredFeatures[this->_registeredFeaturesCount] = newCommand;
        this->_registeredFeaturesCount++;

        Logger::GetInstance()->Info("Registered feature: " + newCommand.GetFeatureName());
    }

    /**
     * TODO: Check if its OK
     */
    String *GetFeatureNames()
    {
        return this->_featureNames;
    }

    void SetupFeatures()
    {
        for (uint8_t i = 0; i < this->_registeredFeaturesCount; i++)
        {
            this->RegisteredFeatures[i].Setup();
        }
    }

    Feature RegisteredFeatures[FEATURES_SIZE];

    static FeatureRegistry *GetInstance()
    {
        if (instance == nullptr)
        {
            FeatureRegistry *createdInstance = new FeatureRegistry();
            instance = createdInstance;
            createdInstance->RegisterFeature(*Logging);
            createdInstance->RegisterFeature(*DisplayFeature);
            createdInstance->RegisterFeature(*SerialRead);
        }

        return instance;
    }
    
};

FeatureRegistry *FeatureRegistry::instance = nullptr;
