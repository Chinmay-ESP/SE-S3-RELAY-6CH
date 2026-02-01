#include "SE_GPIO.hpp"

/*************************************************************  INTERNAL BUZZER STATE  *************************************************************/
static volatile bool buzzerActive = false;
static volatile uint32_t buzzerOffTime = 0;

/*************************************************************  GPIO HELPERS  *************************************************************/
void digitalToggle(int pin)
{
  digitalWrite(pin, !digitalRead(pin));
}

void RGB_Light(uint8_t r, uint8_t g, uint8_t b)
{
  neopixelWrite(GPIO_PIN_RGB, g, r, b);
}

/*************************************************************  BUZZER (NON-BLOCKING)  *************************************************************/
void Buzzer_Trigger(uint16_t durationMs)
{
  ledcWrite(PWM_Channel, Dutyfactor);
  buzzerOffTime = millis() + durationMs;
  buzzerActive = true;
}

void Buzzer_Task()
{
  if (buzzerActive && millis() >= buzzerOffTime) {
    ledcWrite(PWM_Channel, 0);
    buzzerActive = false;
  }
}

/*************************************************************  GPIO INIT  *************************************************************/
void GPIO_Init()
{
  // Relay outputs
  pinMode(GPIO_PIN_CH1, OUTPUT);
  pinMode(GPIO_PIN_CH2, OUTPUT);
  pinMode(GPIO_PIN_CH3, OUTPUT);
  pinMode(GPIO_PIN_CH4, OUTPUT);
  pinMode(GPIO_PIN_CH5, OUTPUT);
  pinMode(GPIO_PIN_CH6, OUTPUT);

  // RGB + Buzzer
  pinMode(GPIO_PIN_RGB, OUTPUT);
  pinMode(GPIO_PIN_Buzzer, OUTPUT);

  // Default OFF
  digitalWrite(GPIO_PIN_CH1, LOW);
  digitalWrite(GPIO_PIN_CH2, LOW);
  digitalWrite(GPIO_PIN_CH3, LOW);
  digitalWrite(GPIO_PIN_CH4, LOW);
  digitalWrite(GPIO_PIN_CH5, LOW);
  digitalWrite(GPIO_PIN_CH6, LOW);

  // PWM init
  ledcSetup(PWM_Channel, Frequency, Resolution);
  ledcAttachPin(GPIO_PIN_Buzzer, PWM_Channel);
  ledcWrite(PWM_Channel, 0);
}
