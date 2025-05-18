#pragma once
#include <WiFiUdp.h>
#include <stdio.h>
#include <time.h>
#include "../Feature.h"

#ifdef ESP32

#define MY_NTP_SERVER "pool.ntp.org"
#define MY_GMT_OFFSET 3600
#define MY_DST_OFFSET 3600

#else

#define MY_NTP_SERVER "pool.ntp.org"
#define MY_TZ "CET-1CEST,M3.5.0,M10.5.0/3"

#endif

time_t getEpochTime()
{
  time_t now;
  time(&now);
  return now;
}

String getUtcTime()
{
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, 80, "%FT%TZ", timeinfo);
  return String(buffer);
}

Feature *TimeFeature = new Feature("Time", []()
                                   {
#ifdef ESP32
    configTime(MY_GMT_OFFSET, MY_DST_OFFSET, MY_NTP_SERVER);
#else
    configTime(MY_TZ, MY_NTP_SERVER);
#endif
    return FeatureState::RUNNING; }, []() {});
