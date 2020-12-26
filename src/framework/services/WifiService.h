#if !defined(_WifiService_) && (defined(ESP8266) || defined(ESP32))
#define _WifiService_

#include <Arduino.h>
#include <framework/common/Event.h>

using namespace std;

namespace Services
{
  namespace Wifi
  {

    void Initialize();

    void EnableWifi(string ssid, string password, string hostname);

    void DisableWifi();

    bool IsEnabled();

    bool IsConnected();

    extern Event<void> WifiEnablingEvent;

    extern Event<void> WifiDisablingEvent;

    extern Event<void> AccessPointConnectedEvent;

    extern Event<void> AccessPointDisconnectedEvent;

  } // namespace Wifi
} // namespace Services

#endif
