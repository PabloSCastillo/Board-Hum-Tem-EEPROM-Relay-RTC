#pragma once
#include <Arduino.h>

namespace Display
{
  void begin();
  void showTime(uint8_t hour, uint8_t minute, uint8_t second);
  void showConnection(bool connected);
  void showAir(float temperature, float humidity);
  void showLightHours(uint8_t hourOn, uint8_t hourOff);
}
