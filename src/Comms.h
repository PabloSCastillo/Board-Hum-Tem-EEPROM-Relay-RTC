#pragma once
#include <Arduino.h>

namespace Comms
{
  // Se llama al llegar un mensaje MQTT: topic y valor numérico del payload
  using MessageHandler = void (*)(const char *topic, int value);

  void begin(MessageHandler handler); // WiFi + MQTT
  void update();                      // llamar en cada loop(): reconecta y procesa mensajes
  bool isConnected();                 // estado del broker MQTT
  bool publishFloat(const char *topic, float value);
}
