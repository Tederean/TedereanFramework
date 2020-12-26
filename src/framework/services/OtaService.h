#if !defined(_OtaService_) && (defined(ESP8266) || defined(ESP32))
#define _OtaService_

#include <Arduino.h>
#include <framework/common/Event.h>

using namespace std;

namespace Services
{
  namespace Ota
  {

    void Initialize();

    void EnableOta(string hostname, string password);

    void DisableOta();

    bool IsEnabled();

    bool IsUpdating();

    extern Event<void> UpdateStartEvent;

    extern Event<void> UpdateEndEvent;

    extern Event<void> UpdateErrorEvent;

  } // namespace Ota
} // namespace Services

#endif
