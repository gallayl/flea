#pragma once
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <stdio.h>
#include <time.h> 
#include "../Feature.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

time_t getNtpTime(){
    timeClient.update();
    return timeClient.getEpochTime();
}

String getUtcTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"%FT%TZ",timeinfo);
  return String(buffer);
}


Feature *TimeFeature = new Feature("Time", []() {

    timeClient.begin();
    return FeatureState::RUNNING;
}, [](){});
