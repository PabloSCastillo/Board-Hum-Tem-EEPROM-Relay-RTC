#include "Sensors.h"
#include <Adafruit_AHTX0.h>
#include "config.h"

namespace Sensors
{
  static Adafruit_AHTX0 aht;

  bool begin()
  {
    pinMode(PIN_SOIL_HUM, INPUT);
    return aht.begin();
  }

  Reading readAir()
  {
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    return {temp.temperature, humidity.relative_humidity};
  }

  int readSoil() { return analogRead(PIN_SOIL_HUM); }
}
