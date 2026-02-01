#ifndef BLE_RELAY_HPP
#define BLE_RELAY_HPP

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BLE_Mode  2

/* ================= CUSTOM UUIDs ================= */
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define RX_UUID      "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define TX_UUID      "beb5484a-36e1-4688-b7f5-ea07361b26a8"

/* ================= GLOBALS ================= */
extern bool deviceConnected;
extern BLECharacteristic *txChar;

/* ===== BLE COMMAND PIPE ===== */
extern volatile bool bleCmdPending;
extern volatile uint8_t bleCmdValue;

/* ================= FUNCTIONS ================= */
void BLE_Relay_Init();
void BLE_Task();   // <-- call in loop()

/* ===== SHARED STATE ===== */
extern bool Relay_Flag[6];

/* ===== CORE RELAY LOGIC ===== */
void Relay_Analysis(uint8_t *buf, uint8_t Mode_Flag);

/* ================= CALLBACK CLASSES ================= */
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer*) override;
  void onDisconnect(BLEServer*) override;
};

class RXCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) override;
};

#endif
