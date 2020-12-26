#ifndef _SystemService_
#define _SystemService_

#include <Arduino.h>
#include <framework/common/Event.h>

using namespace std;

#ifdef SERIAL_DEBUG
#define Debug(x) Serial.print(x)
#else
#define Debug(x)
#endif

#ifdef SERIAL_DEBUG
#define DebugFlush(x) Serial.flush()
#else
#define DebugFlush(x)
#endif

#if defined(ESP32)
typedef int64_t timespan_t;
#elif defined(ESP8266)
typedef uint64_t timespan_t;
#else
typedef uint32_t timespan_t;
#endif


namespace Services
{
  namespace System
  {

    void Initialize();

    timespan_t GetUptime_us();

    void InvokeLater(Event<void> *event, timespan_t delay_us, bool repeating);

    void InvokeCancel(Event<void> *event);

    extern Event<void> LoopEvent;

  } // namespace System
} // namespace Services

#endif
