cat << 'EOF' > README.md
# ESP32 Wireless Environmental Controller & Telemetry

Sistema embebido basado en **ESP32 DevKit** para el monitoreo ambiental continuo, control programado de iluminación y transmisión de telemetría en tiempo real sobre **MQTT**. 

El dispositivo gestiona un conjunto de periféricos en un bus común **I²C** (display OLED, sensor higrotérmico de precisión, RTC de respaldo y memoria no volátil EEPROM), integrando almacenamiento persistente para tolerar cortes de energía sin pérdida de configuración.

---

## Características Principales

- **Adquisición Ambiental:** Lectura periódica de temperatura ambiente y humedad relativa mediante sensor **AHT10 / AHT20**.
- **Monitoreo de Humedad en Suelo:** Entrada analógica dedicada en ADC1 (GPIO 36 / SENSOR_VP) para sensado de sustrato sin conflicto de RF/Wi-Fi.
- **Reloj en Tiempo Real (RTC DS3231):** Mantiene la base de tiempo y sincronización local independiente de caídas de red o reinicios del microcontrolador.
- **Interfaz Visual (OLED SH1106 / SSD1306):** Renderizado de hora en vivo, lecturas ambientales, estado de conexión Wi-Fi/MQTT y horarios configurados mediante la librería optimizada `U8x8`.
- **Persistencia en EEPROM Externa (AT24C32 / 24Cxx):** Almacenamiento no volátil de los umbrales horarios para encendido y apagado de luces (dirección `0x50`).
- **Telemetría Bidireccional MQTT:** Publicación periódica de métricas y suscripción a tópicos de comando para reconfiguración remota de horarios en caliente.

---

## Arquitectura de Hardware y Pines

Todos los dispositivos digitales operan bajo el bus maestro por hardware del ESP32 a 3.3V:

| Dispositivo | Interfaz / Pin | Pin ESP32 (GPIO) | Descripción / Dirección I²C |
| :--- | :--- | :--- | :--- |
| **OLED (SH1106 / SSD1306)** | I²C SDA / SCL | **GPIO 21 / GPIO 22** | Pantalla gráfica 128x64 (`0x3C`) |
| **AHT10 / AHT20** | I²C SDA / SCL | **GPIO 21 / GPIO 22** | Sensor de temperatura y humedad (`0x38`) |
| **RTC DS3231** | I²C SDA / SCL | **GPIO 21 / GPIO 22** | Reloj en tiempo real de alta precisión (`0x68`) |
| **EEPROM AT24C32** | I²C SDA / SCL | **GPIO 21 / GPIO 22** | Memoria externa I²C (`0x50` o `0x57`) |
| **Sensor Humedad Suelo** | Analógico | **GPIO 36 (ADC1_CH0)** | Entrada analógica protegida (Input-only) |
| **Alimentación Lógica** | VCC / GND | **3V3 / GND** | Riel de 3.3 V regulado |

---

## Tópicos MQTT

El cliente publica y suscribe con el ID configurado (`userFlidoor`).

### Telemetría Saliente (Publish)
* `esp32/temperature` : Temperatura ambiente actual en grados Celsius (`float`, formato `24.50`).
* `esp32/humidity` : Humedad relativa ambiente (`float`, formato `58.20`).

### Comandos Entrantes (Subscribe)
* `esp32/hourTurnOnLight` : Define la hora entera (formato 24h, `0` a `23`) de encendido. Actualiza la EEPROM y la pantalla al instante.
* `esp32/hourTurnOffLight` : Define la hora entera (formato 24h, `0` a `23`) de apagado. Actualiza la EEPROM y la pantalla al instante.

---

## Configuración y Entorno de Desarrollo

El firmware está estructurado para compilarse y desplegarse mediante **PlatformIO** (VS Code / CLI).

### Archivo `platformio.ini`

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
board_build.flash_mode = dio
board_build.f_flash = 40000000L

lib_deps = 
    olikraus/U8g2@^2.35.19
    knolleary/PubSubClient@^2.8
    adafruit/Adafruit AHTX0@^2.0.5
    adafruit/RTClib@^2.1.4
    adafruit/Adafruit BusIO
    adafruit/Adafruit Unified Sensor