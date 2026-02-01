#include "SE_Serial.hpp"

/* Use UART1 */
HardwareSerial RS485Serial(1);

void Serial_Init()
{
  RS485Serial.begin(9600, SERIAL_8N1, RXD1, TXD1);
  Serial.println("RS485 Simple Control Ready");
}

void Serial_Loop()
{
  if (RS485Serial.available())
  {
    uint8_t cmd = RS485Serial.read();

    Serial.print("RS485 RX: ");
    Serial.println(cmd);

    /* Accept only valid commands */
    if (cmd >= CH1 && cmd <= ALL_OFF)
    {
      uint8_t buf[1];
      buf[0] = cmd;

      Relay_Analysis(buf, RS485_Mode);
    }
    else
    {
      Serial.println("Invalid RS485 Command");
    }
  }
}
