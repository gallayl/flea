#pragma once

#include "FS.h"
#include "LittleFS.h"
#include "./Logger.h"

#include <SimpleFTPServer.h>

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
        ftpSrv.setLocalIp(WiFi.localIP());
        ftpSrv.begin(FTP_USER, FTP_PASS);
        isStorageAvailable = true;
    }
}