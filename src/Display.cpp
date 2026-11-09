#include "Display.h"
#include <U8x8lib.h>

namespace Display
{
  static U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE);
  static bool connDrawn = false;
  static bool lastConn = false;

  // Iconos 8x8 (cada byte es una columna, el bit 0 es el pixel superior)
  static uint8_t iconWifi[8] = {0xE0, 0xE0, 0x00, 0xF8, 0xF8, 0x00, 0xFF, 0xFF}; // 3 barras rellenas
  static uint8_t iconNoConn[8] = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};

  void begin()
  {
    u8x8.begin();
    u8x8.setPowerSave(0);
    u8x8.clear();
  }

  void showTime(uint8_t hour, uint8_t minute, uint8_t second)
  {
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", (int)hour, (int)minute, (int)second);
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(4, 0, buf);
  }

  void showConnection(bool connected)
  {
    if (connDrawn && connected == lastConn)
      return;
    connDrawn = true;
    lastConn = connected;
    u8x8.drawTile(14, 0, 1, connected ? iconWifi : iconNoConn);
  }

  void showAir(float temperature, float humidity)
  {
    char buf[8];
    u8x8.setFont(u8x8_font_profont29_2x3_r);
    snprintf(buf, sizeof(buf), "T %-2d", (int)lroundf(temperature));
    u8x8.drawString(0, 2, buf);
    snprintf(buf, sizeof(buf), "H %-2d", (int)lroundf(humidity));
    u8x8.drawString(0, 5, buf);
  }

  void showLightHours(uint8_t hourOn, uint8_t hourOff)
  {
    char buf[8];
    u8x8.setFont(u8x8_font_open_iconic_weather_2x2);
    u8x8.drawGlyph(9, 2, 69);
    u8x8.drawGlyph(9, 5, 66);

    u8x8.setFont(u8x8_font_8x13_1x2_r);
    snprintf(buf, sizeof(buf), "%02d", (int)hourOn);
    u8x8.drawString(12, 2, buf);
    snprintf(buf, sizeof(buf), "%02d", (int)hourOff);
    u8x8.drawString(12, 5, buf);

    u8x8.setFont(u8x8_font_5x7_r);
    u8x8.drawString(14, 3, "H");
    u8x8.drawString(14, 6, "H");
  }
}