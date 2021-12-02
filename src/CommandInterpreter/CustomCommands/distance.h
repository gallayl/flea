#pragma once

#include "NewPing.h"

#include "../CommandParser.h"
#include "../CustomCommand.h"

int trigPin = D5;
int echoPin = D8;

// NewPing sonar(trigPin, echoPin);

CustomCommand *distanceAction = new CustomCommand("distance", [](String command) {
    float distance = 0; //sonar.convert_cm(sonar.ping_median(5));
    return String("Distance:" + String(distance) + "cm");
});
