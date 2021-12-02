#pragma once

#include <ESP8266FtpServer.h>
#ifdef ESP32
#include "LittleFS.h"
#else
#include "LittleFS.h"

#endif
#include "./Logger.h"

#define FTP_USER "admin"
#define FTP_PASS "admin"

FtpServer ftpSrv;

bool isStorageAvailable = false;

void initFtpServer()
{
    logInfo("Setting up FTP Server...");
#ifdef ESP32
    if (LittleFS.begin(true))
#else
    if (LittleFS.begin())
#endif
    {
        ftpSrv.begin(FTP_USER, FTP_PASS);
        isStorageAvailable = true;
    }
    else
    {
        logInfo("FS not available");
    }
}

void handleFtpServer()
{
    if (isStorageAvailable)
    {
        ftpSrv.handleFTP();
    }
}