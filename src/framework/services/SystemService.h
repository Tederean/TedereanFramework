#ifndef _SystemService_
#define _SystemService_

#include <Arduino.h>
#include <framework/common/Event.h>

using namespace std;

#ifdef SERIAL_DEBUG
#define Debug(x) SERIAL_DEBUG_PORT.print(x)
#define DebugFlush(x) SERIAL_DEBUG_PORT.flush()
#else
#define Debug(x)
#define DebugFlush(x)
#endif

#if defined(ESP32)
typedef int64_t timespan_t;
#elif defined(ESP8266)
typedef uint64_t timespan_t;
#else
typedef uint32_t timespan_t;
#endif

enum class SchedulingBehaviour
{
  OneShot,
  FixedPeriod,
  FixedPeriodSkipTicks,
  FixedPause,
};


namespace Services
{
  namespace System
  {

    void Initialize();

#ifdef ESP32
    IRAM_ATTR timespan_t GetUptime_us();
#else
    timespan_t GetUptime_us();
#endif

    void InvokeLater(Event<void> *event, timespan_t delay_us, SchedulingBehaviour schedulingBehaviour);

    void InvokeCancel(Event<void> *event);

    extern Event<void> LoopEvent;

  } // namespace System
} // namespace Services

#endif
