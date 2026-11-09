#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "Display.h"
#include "Sensors.h"
#include "Storage.h"
#include "Rtc.h"
#include "Comms.h"

uint8_t hourOn = 0;
uint8_t hourOff = 0;
int lastSecondSent = -1;

// Mensajes MQTT entrantes: horas de encendido/apagado de la luz
void onMessage(const char *topic, int value)
{
  if (value < 0 || value > 24)
    return;

  if (strcmp(topic, TOPIC_LIGHT_ON) == 0 && value != hourOn)
  {
    Storage::writeByte(EEPROM_ADDR_LIGHT_ON, value);
    hourOn = Storage::readHour(EEPROM_ADDR_LIGHT_ON);
    Display::showLightHours(hourOn, hourOff);
  }
  else if (strcmp(topic, TOPIC_LIGHT_OFF) == 0 && value != hourOff)
  {
    Storage::writeByte(EEPROM_ADDR_LIGHT_OFF, value);
    hourOff = Storage::readHour(EEPROM_ADDR_LIGHT_OFF);
    Display::showLightHours(hourOn, hourOff);
  }
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  Wire.begin(PIN_SDA, PIN_SCL);
  Display::begin();

  if (!Sensors::begin())
  {
    Serial.println("No se encontro el AHT10/AHT20");
    while (1)
      delay(10);
  }
  if (!Rtc::begin())
  {
    Serial.println("No se encontro el RTC");
    while (1)
      delay(10);
  }

  hourOn = Storage::readHour(EEPROM_ADDR_LIGHT_ON);
  hourOff = Storage::readHour(EEPROM_ADDR_LIGHT_OFF);
  Display::showLightHours(hourOn, hourOff);

  Comms::begin(onMessage);
}

void loop()
{
  Comms::update();
  Display::showConnection(Comms::isConnected());

  Sensors::Reading air = Sensors::readAir();
  DateTime now = Rtc::now();

  Display::showTime(now.hour(), now.minute(), now.second());
  Display::showAir(air.temperature, air.humidity);

  // Publica una vez cada PUBLISH_PERIOD_S segundos
  if (now.second() % PUBLISH_PERIOD_S == 0 && now.second() != lastSecondSent)
  {
    lastSecondSent = now.second();
    Comms::publishFloat(TOPIC_TEMPERATURE, air.temperature);
    Comms::publishFloat(TOPIC_HUMIDITY, air.humidity);
  }

  delay(50);
}