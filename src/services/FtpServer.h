#pragma once

#include <ESP8266FtpServer.h>
#ifdef ESP32
#include "SPIFFS.h"
#else 
#include "FS.h"
#endif
#include "./Logger.h"

#define FTP_USER "admin"
#define FTP_PASS "admin"

FtpServer ftpSrv;

bool isStorageAvailable = false;

void initFtpServer() {
    logInfo( "Setting up FTP Server...");
    if (SPIFFS.begin()) {
        ftpSrv.begin(FTP_USER, FTP_PASS);
        isStorageAvailable = true;
    }
}