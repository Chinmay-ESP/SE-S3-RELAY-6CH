# SmartElex ESP32-S3 Relay 6CH Controller

### Multi-Interface Industrial Relay Control System

> A modular, industrial-ready relay controller built on **Waveshare ESP32-S3-Relay-6CH**, supporting **Web UI, Bluetooth Low Energy (BLE), RS485 and Blynk Cloud** control simultaneously.

---

## 📌 Project Overview

This project implements a **robust, scalable and professional relay automation system** using the **ESP32-S3** platform.
It is designed for **industrial panels, smart automation systems, IoT gateways and educational prototyping**.

The system allows **local and remote relay control** through multiple independent interfaces while maintaining **safe command arbitration and non-blocking execution**.

---

## ✨ Key Highlights

* 6-Channel Relay Control
* Simultaneous operation of:

  * 🌐 Web Server (Wi-Fi)
  * 📱 Bluetooth Low Energy (BLE)
  * 🏭 RS485 (Industrial Wired Interface)
  * ☁ Blynk Cloud (Remote IoT)
* Captive Portal Wi-Fi provisioning
* mDNS access (`http://smartelex.local`)
* ASCII-based command protocol (simple & reliable)
* Non-volatile storage of credentials
* Modular library-based architecture (PlatformIO)

---

## 🧩 Target Hardware

| Parameter      | Details                          |
| -------------- | -------------------------------- |
| Board          | **Waveshare ESP32-S3-Relay-6CH** |
| MCU            | ESP32-S3                         |
| Relay Channels | 6                                |
| Interfaces     | Wi-Fi, BLE, RS485                |
| Development    | PlatformIO                       |
| Framework      | Arduino                          |

🔗 Reference:
[https://www.waveshare.com/wiki/ESP32-S3-Relay-6CH](https://www.waveshare.com/wiki/ESP32-S3-Relay-6CH)

---

## 📂 Project Directory Structure

```
.
├── src/
│   └── main.cpp
│
├── lib/
│   ├── SE_Bluetooth/      # BLE services & command handling
│   ├── SE_GPIO/           # Relay, buzzer, RGB GPIO abstraction
│   ├── SE_Serial/         # RS485 / UART communication
│   ├── SE_WIFI/           # WiFiManager, Web Server, mDNS
│   ├── SE_RTC/            # Time and RTC handling
│   ├── SE_Information/    # Device/system information
│
├── platformio.ini
└── README.md
```

✔ Modular design
✔ Clean separation of responsibilities
✔ Easy maintenance and scalability

---

## ⚙️ PlatformIO Configuration

```ini
[env:esp32-s3-devkitm-1]
platform = espressif32@^6.8.0
board = esp32-s3-devkitm-1
framework = arduino

lib_deps =
    tzapu/WiFiManager@^2.0.17
    arduino-libraries/NTPClient@^3.2.1
    blynkkk/Blynk@^1.3.2
    https://github.com/blynkkk/blynk-library
    bblanchon/ArduinoJson@^6.21.3
```

---

## 🌐 Web Interface (Wi-Fi)

### 🔹 Purpose

Local web-based control and configuration without requiring cloud connectivity.

---

### 1️⃣ Access Point Mode (First Boot)

On first startup (or after reset), the device starts in **AP mode**:

```cpp
#define APSSID "SmartElex-Relay-6CH"
#define APPSK  "12345678"
```

* User connects via mobile or laptop
* Captive portal opens automatically

---

### 2️⃣ Wi-Fi Provisioning Stack

Libraries used:

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
```

* Enter Wi-Fi SSID and password
* Credentials stored internally
* Device switches to **Station (STA) mode**

---

### 3️⃣ Local Web Dashboard (mDNS)

After successful Wi-Fi connection:

```
http://smartelex.local
```

📌 Reference Web UI:
**[http://smartelex.local/](http://smartelex.local/)**

---

### 4️⃣ Web Control Capabilities

* Toggle Relay 1 – Relay 6
* ALL ON / ALL OFF
* View system status
* Works on mobile, tablet, and desktop
* No internet required

✔ Ideal for **local automation & on-site monitoring**

---

## 📡 Bluetooth Low Energy (BLE)

### 🔹 Purpose

Local wireless control **without Wi-Fi or internet dependency**.

> Recommended for **commissioning, diagnostics, and maintenance**.

---

### 🔹 BLE Device Information

| Parameter   | Value               |
| ----------- | ------------------- |
| Device Name | `ESP32S3-Relay-6CH` |
| Protocol    | BLE UART / Notify   |
| Usage       | Local Control       |
| Client App  | Serial Bluetooth Terminal       |

---

### 🔹 Supported BLE Commands (ASCII)

| Command   | Action                |
| --------- | --------------------- |
| `1` – `6` | Toggle Relay Channels |
| `7`       | Turn ALL Relays ON    |
| `8`       | Turn ALL Relays OFF   |

✔ Commands sent as **single ASCII characters**
✔ Compatible with generic BLE UART applications

---

## 🏭 RS485 Interface (Industrial Communication)

### 🔹 Purpose

Reliable, noise-immune wired communication for **industrial environments**.

> Recommended for factory floors, control panels and long-distance wiring.

---

### 🔹 Communication Parameters

| Parameter    | Value     |
| ------------ | --------- |
| Signal Lines | A, B, GND |
| Baud Rate    | 9600 bps  |
| Frame Format | 8-N-1     |

---

### 🔹 Supported RS485 Commands (ASCII)

| Command   | Action                   |
| --------- | ------------------------ |
| `1` – `6` | Toggle Individual Relays |
| `7`       | ALL Relays ON            |
| `8`       | ALL Relays OFF           |

✔ Compatible with PLCs, HMIs, USB-RS485 converters
✔ Long-distance & EMI-resistant

---

## ☁ Cloud Setup – Blynk IoT

### 🔹 Purpose

Secure **remote relay control** via internet using Blynk Cloud.

---

### 🔐 Required Credentials

After creating a **Template** and **Device** in the Blynk Console, copy the generated credentials:

```
Template ID   : TMPLxxxxxxx
Template Name : SmartElex Relay 6CH
Auth Token    : Paste your Blynk Auth Token
```

📌 **No code changes required**
📌 Credentials are stored in **non-volatile memory**

---

### 🔗 Connection Flow

1. Create Template in Blynk Console
2. Add Device under the template
3. Copy Template ID, Name, and Auth Token
4. Paste credentials into device configuration
5. Click **Connect**

✔ Auto-reconnect after reboot
✔ Cloud optional (local control still works)

---

### 📘 Blynk Configuration

#### 🔌 Virtual Pin Mapping

| Virtual Pin | Function          |
| ----------- | ----------------- |
| V1 – V6     | Relay 1 – Relay 6 |
| V7          | ALL ON / ALL OFF  |

---

#### 📱 Widget Setup

| Setting     | Value  |
| ----------- | ------ |
| Widget Type | Button |
| Mode        | Switch |
| ON          | `1`    |
| OFF         | `0`    |

✔ Supported on **Blynk Mobile App**
✔ Supported on **Blynk Web Dashboard**

---

## 🔄 Command Handling & Safety

* All interfaces generate **non-blocking commands**
* Commands are queued using shared flags
* Relay execution happens in the main loop
* Prevents watchdog resets and race conditions

✔ Safe multi-interface coexistence

---

## 🧪 Tested Interfaces

* Web UI (LAN)
* BLE UART apps
* USB-RS485 converters
* Blynk Mobile & Web

---

## 🚀 Applications

* Smart Relay Panels
* Industrial Automation
* IoT Switchboards
* Factory Control Systems
* Educational & Research Projects

---

## 📜 License

```
MIT License

Copyright (c) 2026
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files, to deal in the Software
without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies.
```

---

##  Author

**Chinmay Chavan**
Embedded Systems | IoT | Industrial Automation

---
