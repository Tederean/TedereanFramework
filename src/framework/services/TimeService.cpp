#if defined(ESP8266) || defined(ESP32)

#include <Arduino.h>
#include <ezTime.h>
#include <framework/services/TimeService.h>
#include <framework/services/SystemService.h>

using namespace std;

namespace Services
{
  namespace Time
  {

    void Initialize();

    bool IsTimeSynced();

    void OnLoopEvent(void *args);

    void OnLoopEventTimeSync(void *args);

    Timezone Localtime;

    Event<void> TimeSyncedEvent;

    bool TimeSynced;

    void Initialize()
    {
      TimeSynced = false;

      Localtime.setPosix(TZ_INFO);
      Localtime.setDefault();

#ifdef SERIAL_DEBUG
      ezt::setDebug(INFO);
#endif

      ezt::setServer(TZ_NTP_SERVER);

      Services::System::LoopEvent.Subscribe(OnLoopEvent);
      Services::System::LoopEvent.Subscribe(OnLoopEventTimeSync);
    }

    bool IsTimeSynced()
    {
      return TimeSynced;
    }

    void OnLoopEvent(void *args)
    {
      ezt::events();
    }

    void OnLoopEventTimeSync(void *args)
    {
      if (ezt::timeStatus() == timeStatus_t::timeSet)
      {
        TimeSynced = true;

        ezt::setInterval(0);

        Services::System::LoopEvent.Unsubscribe(OnLoopEventTimeSync);

        TimeSyncedEvent.Invoke(nullptr);
      }
    }

  } // namespace Time
} // namespace Services

#endif