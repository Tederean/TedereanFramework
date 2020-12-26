#if !defined(_TimerService_) && (defined(ESP8266) || defined(ESP32))
#define _TimerService_

#include <Arduino.h>
#include <ezTime.h>
#include <framework/common/Event.h>

using namespace std;

namespace Services
{
  namespace Time
  {

    void Initialize();

    bool IsTimeSynced();

    extern Timezone Localtime;
    
    extern Event<void> TimeSyncedEvent;

  } // namespace Wifi
} // namespace Services

#endif
