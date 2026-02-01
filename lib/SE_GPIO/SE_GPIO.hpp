#ifndef SE_GPIO_HPP
#define SE_GPIO_HPP

#include <Arduino.h>

/*************************************************************  UART  *************************************************************/
#define TXD1 17
#define RXD1 18

/*************************************************************  RELAY GPIO  *************************************************************/
#define GPIO_PIN_CH1      1
#define GPIO_PIN_CH2      2
#define GPIO_PIN_CH3      41
#define GPIO_PIN_CH4      42
#define GPIO_PIN_CH5      45
#define GPIO_PIN_CH6      46

/*************************************************************  RGB + BUZZER  *************************************************************/
#define GPIO_PIN_RGB      38
#define GPIO_PIN_Buzzer   21

/*************************************************************  BUZZER PWM  *************************************************************/
#define PWM_Channel     1
#define Frequency       1000
#define Resolution      8
#define Dutyfactor      200

/*************************************************************  API  *************************************************************/
void digitalToggle(int pin);
void RGB_Light(uint8_t r, uint8_t g, uint8_t b);

void GPIO_Init();

/* ===== NON-BLOCKING BUZZER ===== */
void Buzzer_Trigger(uint16_t durationMs);
void Buzzer_Task();        // must be called in loop()

#endif
