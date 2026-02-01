/********************* BLYNK (REQUIRED DUMMY) *********************/
#define BLYNK_TEMPLATE_ID   "DYNAMIC_TEMPLATE"
#define BLYNK_TEMPLATE_NAME "Dynamic Config"

/********************* INCLUDES *********************/
#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Preferences.h>



#include "SE_WIFI.hpp"
#include "SE_Bluetooth.hpp"
#include "SE_GPIO.hpp"
#include "SE_Serial.hpp"
#include "SE_RTC.hpp"
#include "SE_Information.hpp"

/********************* MODES *********************/
#define RS485_Mode        1
#define Bluetooth_Mode    2
#define WIFI_Mode         3


volatile bool cmdPending = false;
volatile uint8_t pendingCmd = 0;
volatile uint8_t pendingSrc = 0;

/********************* GLOBALS *********************/
Preferences prefs;

bool Relay_Flag[6] = {0};
uint32_t Simulated_time = 0;

/* ---- Blynk runtime ---- */
char blynkToken[40]   = {0};
char templateID[32]   = {0};
char templateName[40] = {0};
bool blynkConfigured  = false;

/********************************************************
 *               START BLYNK
 ********************************************************/
void startBlynk()
{
  if (!blynkConfigured) return;

  Blynk.config(blynkToken);
  if (Blynk.connect(10000)) {
    for (int i = 0; i < 6; i++)
      Blynk.virtualWrite(V1 + i, Relay_Flag[i]);
  }
}

/********************************************************
 *               LOAD CONFIG
 ********************************************************/
void loadBlynkConfig()
{
  prefs.begin("blynk", true);
  if (prefs.isKey("token")) {
    prefs.getString("token", blynkToken, sizeof(blynkToken));
    blynkConfigured = true;
  }
  prefs.end();
}

/********************************************************
 *               RELAY ANALYSIS
 ********************************************************/
void Relay_Analysis(uint8_t *buf, uint8_t Mode_Flag)
{
  switch (buf[0]) {
    case CH1: digitalToggle(GPIO_PIN_CH1); Relay_Flag[0] ^= 1; break;
    case CH2: digitalToggle(GPIO_PIN_CH2); Relay_Flag[1] ^= 1; break;
    case CH3: digitalToggle(GPIO_PIN_CH3); Relay_Flag[2] ^= 1; break;
    case CH4: digitalToggle(GPIO_PIN_CH4); Relay_Flag[3] ^= 1; break;
    case CH5: digitalToggle(GPIO_PIN_CH5); Relay_Flag[4] ^= 1; break;
    case CH6: digitalToggle(GPIO_PIN_CH6); Relay_Flag[5] ^= 1; break;

    case ALL_ON:
      digitalWrite(GPIO_PIN_CH1, HIGH);
      digitalWrite(GPIO_PIN_CH2, HIGH);
      digitalWrite(GPIO_PIN_CH3, HIGH);
      digitalWrite(GPIO_PIN_CH4, HIGH);
      digitalWrite(GPIO_PIN_CH5, HIGH);
      digitalWrite(GPIO_PIN_CH6, HIGH);
      memset(Relay_Flag, 1, sizeof(Relay_Flag));
      break;

    case ALL_OFF:
      digitalWrite(GPIO_PIN_CH1, LOW);
      digitalWrite(GPIO_PIN_CH2, LOW);
      digitalWrite(GPIO_PIN_CH3, LOW);
      digitalWrite(GPIO_PIN_CH4, LOW);
      digitalWrite(GPIO_PIN_CH5, LOW);
      digitalWrite(GPIO_PIN_CH6, LOW);
      memset(Relay_Flag, 0, sizeof(Relay_Flag));
      break;
  }

  Buzzer_Trigger(100);

  if (blynkConfigured) {
    for (int i = 0; i < 6; i++)
      Blynk.virtualWrite(V1 + i, Relay_Flag[i]);
    Blynk.virtualWrite(V7,
      Relay_Flag[0] && Relay_Flag[1] && Relay_Flag[2] &&
      Relay_Flag[3] && Relay_Flag[4] && Relay_Flag[5]);
  }
}

/********************************************************
 *               BLYNK HANDLERS
 ********************************************************/
BLYNK_WRITE(V1) { pendingCmd = CH1; pendingSrc = WIFI_Mode; cmdPending = true; }
BLYNK_WRITE(V2) { pendingCmd = CH2; pendingSrc = WIFI_Mode; cmdPending = true; }
BLYNK_WRITE(V3) { pendingCmd = CH3; pendingSrc = WIFI_Mode; cmdPending = true; }
BLYNK_WRITE(V4) { pendingCmd = CH4; pendingSrc = WIFI_Mode; cmdPending = true; }
BLYNK_WRITE(V5) { pendingCmd = CH5; pendingSrc = WIFI_Mode; cmdPending = true; }
BLYNK_WRITE(V6) { pendingCmd = CH6; pendingSrc = WIFI_Mode; cmdPending = true; }
BLYNK_WRITE(V7) {
  pendingCmd = param.asInt() ? ALL_ON : ALL_OFF;
  pendingSrc = WIFI_Mode;
  cmdPending = true;
}

/********************************************************
 *               SETUP
 ********************************************************/
void setup()
{
  Serial.begin(115200);

  Serial_Init();     // RS485 / UART
  GPIO_Init();       // Relays, RGB, buzzer

  WIFI_Init();       // WiFiManager + WebServer (BLOCKING)
  delay(300); 
  BLE_Relay_Init();  // BLE AFTER WiFi (CRITICAL FIX)

  loadBlynkConfig();
  if (blynkConfigured) {
    startBlynk();
  }

  Serial.println("System Ready");
}


/********************************************************
 *               LOOP (SAFE EXECUTION)
 ********************************************************/
void loop()
{
  BLE_Task();        // ✅ REQUIRED (fixes reset)
  Buzzer_Task();   // 🔴 REQUIRED
  Serial_Loop();
  WIFI_Loop();

  if (cmdPending) {
    uint8_t c = pendingCmd;
    uint8_t s = pendingSrc;
    cmdPending = false;
    Relay_Analysis(&c, s);
  }

  if (blynkConfigured) Blynk.run();
}
