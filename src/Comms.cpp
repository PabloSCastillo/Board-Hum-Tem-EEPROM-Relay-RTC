#include "Comms.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "secrets.h"

namespace Comms
{
  static WiFiClient wifiClient;
  static PubSubClient mqtt(wifiClient);
  static MessageHandler handler = nullptr;
  static unsigned long lastAttempt = 0;

  static void onMqttMessage(char *topic, byte *payload, unsigned int length)
  {
    char buf[16];
    unsigned int n = (length < sizeof(buf) - 1) ? length : sizeof(buf) - 1;
    memcpy(buf, payload, n);
    buf[n] = '\0';

    Serial.printf("MQTT %s = %s\n", topic, buf);
    if (handler)
      handler(topic, atoi(buf));
  }

  static void tryConnect()
  {
    lastAttempt = millis();
    Serial.print("Conectando a MQTT... ");
    if (mqtt.connect(MQTT_CLIENT_ID))
    {
      Serial.println("conectado");
      mqtt.subscribe(TOPIC_LIGHT_ON);
      mqtt.subscribe(TOPIC_LIGHT_OFF);
    }
    else
    {
      Serial.printf("fallo, estado=%d\n", mqtt.state());
    }
  }

  void begin(MessageHandler h)
  {
    handler = h;

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Conectando a WiFi");
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 20000)
    {
      delay(500);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("\nIP: ");
      Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.println("\nSin WiFi por ahora, se reintenta en segundo plano");
    }

    mqtt.setBufferSize(512);
    mqtt.setServer(MQTT_SERVER, MQTT_PORT);
    mqtt.setCallback(onMqttMessage);
    if (WiFi.status() == WL_CONNECTED)
      tryConnect();
  }

  void update()
  {
    if (WiFi.status() != WL_CONNECTED)
      return; // el ESP32 reconecta el WiFi solo

    if (mqtt.connected())
      mqtt.loop();
    else if (millis() - lastAttempt >= MQTT_RETRY_MS)
      tryConnect();
  }

  bool isConnected() { return mqtt.connected(); }

  bool publishFloat(const char *topic, float value)
  {
    if (!mqtt.connected())
      return false;
    char buf[12];
    dtostrf(value, 1, 2, buf);
    return mqtt.publish(topic, buf);
  }
}
