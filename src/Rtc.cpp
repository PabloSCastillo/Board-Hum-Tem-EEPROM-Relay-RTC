#include "Rtc.h"

namespace Rtc
{
  static RTC_DS3231 rtc;

  bool begin()
  {
    if (!rtc.begin())
      return false;
    if (rtc.lostPower())
    {
      Serial.println("RTC sin hora: ajustando a la hora de compilacion");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    return true;
  }

  DateTime now() { return rtc.now(); }
}
