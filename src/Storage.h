#pragma once
#include <Arduino.h>

namespace Storage
{
  uint8_t readByte(uint16_t address);
  void writeByte(uint16_t address, uint8_t data);
  // Lee una hora (0-24). Si el dato no es válido (EEPROM virgen = 0xFF) devuelve 0.
  uint8_t readHour(uint16_t address);
}
