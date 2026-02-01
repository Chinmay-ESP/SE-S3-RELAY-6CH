#include "SE_Bluetooth.hpp"

/* ================= GLOBALS ================= */
bool deviceConnected = false;
BLECharacteristic *txChar = nullptr;

/* ===== BLE → MAIN COMMAND PIPE ===== */
volatile bool bleCmdPending = false;
volatile uint8_t bleCmdValue = 0;

/* ================= SERVER CALLBACK ================= */
void ServerCallbacks::onConnect(BLEServer*) {
  deviceConnected = true;
}

void ServerCallbacks::onDisconnect(BLEServer*) {
  deviceConnected = false;
  BLEDevice::startAdvertising();
}

/* ================= RX CALLBACK (SAFE) ================= */
void RXCallback::onWrite(BLECharacteristic *pChar)
{
  std::string rxValue = pChar->getValue();
  if (rxValue.empty()) return;

  int cmd = atoi(rxValue.c_str());   // expects 1..8
  if (cmd < 1 || cmd > 8) return;

  // ✅ ONLY STORE COMMAND
  bleCmdValue   = (uint8_t)cmd;
  bleCmdPending = true;
}

/* ================= BLE TASK (RUNS IN loop()) ================= */
void BLE_Task()
{
  if (!bleCmdPending) return;

  uint8_t cmd = bleCmdValue;
  bleCmdPending = false;

  uint8_t buf[1];
  buf[0] = cmd + '0';

  // ✅ SAFE: now runs in Arduino loop context
  Relay_Analysis(buf, BLE_Mode);

  // ===== BLE ACK =====
  if (deviceConnected && txChar) {

    char resp[16];

    if (cmd >= 1 && cmd <= 6)
      snprintf(resp, sizeof(resp), "%d-%s", cmd,
               Relay_Flag[cmd - 1] ? "ON" : "OFF");
    else if (cmd == 7)
      strcpy(resp, "ALL-ON");
    else
      strcpy(resp, "ALL-OFF");

    txChar->setValue(resp);
    txChar->notify();
  }
}

/* ================= INIT ================= */
void BLE_Relay_Init()
{
  BLEDevice::init("ESP32S3-Relay-6CH");

  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService *service = server->createService(SERVICE_UUID);

  txChar = service->createCharacteristic(
    TX_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  txChar->addDescriptor(new BLE2902());

  BLECharacteristic *rxChar = service->createCharacteristic(
    RX_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  rxChar->setCallbacks(new RXCallback());

  service->start();

  BLEAdvertising *adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->start();
}
