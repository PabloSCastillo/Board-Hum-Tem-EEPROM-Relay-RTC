#pragma once
#include <Arduino.h>

namespace Sensors
{
  struct Reading
  {
    float temperature;
    float humidity;
  };

  bool begin();           // AHT10/AHT20 + pin de humedad de suelo
  Reading readAir();      // temperatura (°C) y humedad relativa (%)
  int readSoil();         // valor crudo del ADC (0-4095)
}
