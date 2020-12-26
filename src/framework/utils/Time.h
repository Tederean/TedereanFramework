#if !defined(_Time_) && (defined(ESP8266) || defined(ESP32))
#define _Time_

#include <Arduino.h>
#include <ezTime.h>

using namespace std;

namespace Time
{

  template <typename T>
  T GetSecondsOfDay(time_t time, Timezone *timezone)
  {
    auto hours = timezone->hour(time, UTC_TIME);
    auto minutes = timezone->minute(time, UTC_TIME);
    auto seconds = timezone->second(time, UTC_TIME);

    if (is_same<T, float>::value || is_same<T, double>::value)
    {
      auto millis = timezone->ms(LAST_READ);

      return (hours * 3600.0) + (minutes * 60.0) + (seconds * 1.0) + (millis * 0.001);
    }

    return (hours * 3600) + (minutes * 60) + (seconds * 1);
  }

} // namespace Time

#endif
