#include "Storage.h"
#include <Wire.h>
#include "config.h"

namespace Storage
{
  uint8_t readByte(uint16_t address)
  {
    uint8_t data = 0xFF;
    Wire.beginTransmission(EEPROM_I2C_ADDR);
    Wire.write((uint8_t)(address >> 8));
    Wire.write((uint8_t)(address & 0xFF));
    Wire.endTransmission();
    Wire.requestFrom((uint16_t)EEPROM_I2C_ADDR, (uint8_t)1);
    if (Wire.available())
      data = Wire.read();
    delay(5);
    return data;
  }

  void writeByte(uint16_t address, uint8_t data)
  {
    Wire.beginTransmission(EEPROM_I2C_ADDR);
    Wire.write((uint8_t)(address >> 8));
    Wire.write((uint8_t)(address & 0xFF));
    Wire.write(data);
    Wire.endTransmission();
    delay(5);
  }

  uint8_t readHour(uint16_t address)
  {
    uint8_t value = readByte(address);
    return (value > 24) ? 0 : value;
  }
}
