#pragma once
#include <Arduino.h>

// --- Pines ---
constexpr uint8_t PIN_SDA = 21;
constexpr uint8_t PIN_SCL = 22;
constexpr uint8_t PIN_SOIL_HUM = 36; // ADC1, compatible con WiFi activo

// --- EEPROM I2C (si no responde, probar 0x57) ---
constexpr uint8_t EEPROM_I2C_ADDR = 0x50;
constexpr uint16_t EEPROM_ADDR_LIGHT_ON = 2;
constexpr uint16_t EEPROM_ADDR_LIGHT_OFF = 3;

// --- MQTT ---
constexpr const char *MQTT_CLIENT_ID = "userFlidoor";
constexpr const char *TOPIC_HUMIDITY = "esp32/humidity";
constexpr const char *TOPIC_TEMPERATURE = "esp32/temperature";
constexpr const char *TOPIC_LIGHT_ON = "esp32/hourTurnOnLight";
constexpr const char *TOPIC_LIGHT_OFF = "esp32/hourTurnOffLight";
constexpr unsigned long MQTT_RETRY_MS = 5000;

// --- Temporización ---
constexpr int PUBLISH_PERIOD_S = 10; // publica cuando segundos % 10 == 0
