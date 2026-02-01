#include "SE_Blynk.hpp"

/* ================= GLOBALS ================= */

bool blynkConnected = false;

/* ================= BLYNK CALLBACK ================= */

BLYNK_CONNECTED()
{
  blynkConnected = true;
}

BLYNK_DISCONNECTED()
{
  blynkConnected = false;
}

/* ================= VIRTUAL PINS ================= */
/*
   V1 → Relay 1
   V2 → Relay 2
   V3 → Relay 3
   V4 → Relay 4
   V5 → Relay 5
   V6 → Relay 6
*/

BLYNK_WRITE(V1) { Relay_Control(1, param.asInt()); }
BLYNK_WRITE(V2) { Relay_Control(2, param.asInt()); }
BLYNK_WRITE(V3) { Relay_Control(3, param.asInt()); }
BLYNK_WRITE(V4) { Relay_Control(4, param.asInt()); }
BLYNK_WRITE(V5) { Relay_Control(5, param.asInt()); }
BLYNK_WRITE(V6) { Relay_Control(6, param.asInt()); }

/* ================= INIT ================= */

void Blynk_Init()
{
  Blynk.config(BLYNK_AUTH_TOKEN);

  if (!Blynk.connect(3000)) {
    Serial.println("[BLYNK] Connection Failed");
  } else {
    Serial.println("[BLYNK] Connected");
  }
}

/* ================= LOOP ================= */

void Blynk_Run()
{
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
  }
}
