#ifndef SE_BLYNK_HPP
#define SE_BLYNK_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/* ================= BLYNK CONFIG ================= */

#define BLYNK_PRINT Serial

extern bool blynkConnected;

/* ================= API ================= */

void Blynk_Init();
void Blynk_Run();

/* Relay handler (implemented in main / GPIO layer) */
extern void Relay_Control(uint8_t ch, bool state);

#endif  // SE_BLYNK_HPP
