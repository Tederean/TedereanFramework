#if defined(ESP8266) || defined(ESP32)

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <framework/services/WifiService.h>
#include <framework/services/OtaService.h>
#include <framework/services/SystemService.h>

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

    void BeginOta();

    void EndOta();

    void OnLoopEvent(void *args);

    void OnAccessPointConnectedEvent(void *args);

    void OnAccessPointDisconnectedEvent(void *args);

    string Password;

    string Hostname;

    bool OtaEnabled;

    bool OtaIsUpdating;

    Event<void> UpdateStartEvent;

    Event<void> UpdateEndEvent;

    Event<void> UpdateErrorEvent;

    void Initialize()
    {
      OtaEnabled = false;
      OtaIsUpdating = false;

      ArduinoOTA.onStart([]
      {
        UpdateStartEvent.Invoke(nullptr);

        OtaIsUpdating = true;
      });

      ArduinoOTA.onEnd([]
      {
        UpdateEndEvent.Invoke(nullptr);

        OtaIsUpdating = false;
      });

      ArduinoOTA.onError([](ota_error_t error)
      {
        UpdateErrorEvent.Invoke(nullptr);

        OtaIsUpdating = false;
      });
    }

    void EnableOta(string hostname, string password)
    {
      if (OtaEnabled || OtaIsUpdating)
        return;

      OtaEnabled = true;
      
      Hostname.assign(hostname);
      Password.assign(password);

      if (Services::Wifi::IsConnected())
      {
        BeginOta();
      }

      Services::Wifi::AccessPointConnectedEvent.Subscribe(OnAccessPointConnectedEvent);
      Services::Wifi::AccessPointDisconnectedEvent.Subscribe(OnAccessPointDisconnectedEvent);
    }

    void DisableOta()
    {
      if (!OtaEnabled || OtaIsUpdating)
        return;

      OtaEnabled = false;

      if (Services::Wifi::IsConnected())
      {
        EndOta();
      }

      Services::Wifi::AccessPointConnectedEvent.Unsubscribe(OnAccessPointConnectedEvent);
      Services::Wifi::AccessPointDisconnectedEvent.Unsubscribe(OnAccessPointDisconnectedEvent);

      Hostname.clear();
      Password.clear();
    }

    bool IsEnabled()
    {
      return OtaEnabled;
    }

    bool IsUpdating()
    {
      return OtaIsUpdating;
    }

    void BeginOta()
    {
      ArduinoOTA.setHostname(Hostname.c_str());
      ArduinoOTA.setPassword((const char *)Password.c_str());
      ArduinoOTA.setPort(8266);

#if defined(ESP8266)
      ArduinoOTA.begin(true);
#elif defined(ESP32)
      ArduinoOTA.setMdnsEnabled(true);
      ArduinoOTA.begin();
#endif

      Services::System::LoopEvent.Subscribe(OnLoopEvent);
    }

    void EndOta()
    {
#if defined(ESP32)
      ArduinoOTA.end();
#endif

      Services::System::LoopEvent.Unsubscribe(OnLoopEvent);
    }

    void OnLoopEvent(void *args)
    {
      ArduinoOTA.handle();
    }

    void OnAccessPointConnectedEvent(void *args)
    {
      BeginOta();
    }

    void OnAccessPointDisconnectedEvent(void *args)
    {
      EndOta();
    }

  } // namespace Ota
} // namespace Services

#endif