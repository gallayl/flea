#pragma once
#include <Arduino.h>

typedef void (*FeatureSetupFunction)();

typedef void (*FeatureLoopFunction)();

class Feature
{
public:
    Feature(String name = "featureName", FeatureSetupFunction setupCallback = []() {}, FeatureLoopFunction loopCallback = []() {}) : _featureName(name), _onSetup(setupCallback), _onLoop(loopCallback) {};

    void Setup()
    {
        return this->_onSetup();
    }

    String GetFeatureName()
    {
        return this->_featureName;
    }

protected:
    String _featureName;
    FeatureSetupFunction _onSetup;
    FeatureLoopFunction _onLoop;
};
