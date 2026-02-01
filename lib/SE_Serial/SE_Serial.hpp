#ifndef SE_SERIAL_HPP
#define SE_SERIAL_HPP

#include <Arduino.h>
#include <HardwareSerial.h>
#include "SE_Information.hpp"

/* ========= MODE ========= */
#define RS485_Mode  1

/* ========= UART PINS ========= */
#define TXD1 17
#define RXD1 18

/* ========= API ========= */
void Serial_Init();
void Serial_Loop();

/* ========= RELAY HANDLER ========= */
extern void Relay_Analysis(uint8_t *buf, uint8_t Mode_Flag);

#endif
