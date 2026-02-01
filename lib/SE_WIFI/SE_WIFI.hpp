#ifndef SE_WIFI_HPP
#define SE_WIFI_HPP

#include <WiFi.h>
#include <WebServer.h> 
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include "stdio.h"
#include "SE_GPIO.hpp"
#include "SE_Information.hpp"

#define WIFI_Mode            3

extern bool Relay_Flag[6];       // Relay current status flag
extern void Relay_Analysis(uint8_t *buf,uint8_t Mode_Flag);

void handleRoot();
void handleGetData();
void handleSwitch(uint8_t ledNumber);

void handleSwitch1();
void handleSwitch2();
void handleSwitch3();
void handleSwitch4();
void handleSwitch5();
void handleSwitch6();
void handleSwitch7();
void handleSwitch8();
void WIFI_Init();
void WIFI_Loop();
void startWebServer();
void startMDNS();
void startFallbackAP();


#endif