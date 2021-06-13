#include <Arduino.h>

#if defined(__SAM3X8E__) || defined(__SAMD21G18A__)
#undef min
#undef max
#include <algorithm>
#endif

#include <vector>

#if defined(__AVR__)
#include <ArxSmartPtr.h>
#else
#include <memory>
#endif

#include <framework/services/SystemService.h>
#include <framework/common/Event.h>

using namespace std;

typedef struct
{
  Event<void> *TargetEvent;
  timespan_t LastExecution_us;
  timespan_t Interval_us;
  SchedulingBehaviour Mode;
} ScheduledEvent;

namespace Services
{
  namespace System
  {

    void Initialize();

    timespan_t GetUptime_us();

    void InvokeLater(Event<void> *event, timespan_t delay_us, SchedulingBehaviour schedulingBehaviour);

    void InvokeCancel(Event<void> *event);

    void AddEvent(Event<void> *event, timespan_t firstDelay_us, timespan_t interval_us);

    void RemoveEvent(Event<void> *event);

    int16_t FindEventIndex(Event<void> *eventToFind);

    void OnLoopEvent(void *args);

    Event<void> LoopEvent;

    vector<shared_ptr<ScheduledEvent>> ScheduledTargets;

    void Initialize()
    {
#ifdef SERIAL_DEBUG
      SERIAL_DEBUG_PORT.begin(SERIAL_DEBUG_SPEED);

#if defined(ESP8266) || defined(ESP32)
      SERIAL_DEBUG_PORT.setDebugOutput(true);
#endif
#endif
    }

    timespan_t GetUptime_us()
    {
#if defined(ESP32)
      return esp_timer_get_time();
#elif defined(ESP8266)
      return micros64();
#else
      return micros();
#endif
    }

    void InvokeLater(Event<void> *event, timespan_t delay_us, SchedulingBehaviour schedulingBehaviour)
    {
      int16_t index = FindEventIndex(event);

      if (index < 0)
      {
        shared_ptr<ScheduledEvent> scheduledEvent((ScheduledEvent *)malloc(sizeof(ScheduledEvent)));

        scheduledEvent->TargetEvent = event;
        scheduledEvent->LastExecution_us = GetUptime_us();
        scheduledEvent->Interval_us = delay_us;
        scheduledEvent->Mode = schedulingBehaviour;

        if (ScheduledTargets.empty())
        {
          LoopEvent.Subscribe(OnLoopEvent);
        }

        ScheduledTargets.push_back(scheduledEvent);
      }
    }

    void InvokeCancel(Event<void> *event)
    {
      RemoveEvent(event);
    }

    void RemoveEvent(Event<void> *event)
    {
      int16_t index = FindEventIndex(event);

      if (index >= 0)
      {
        ScheduledTargets.erase(ScheduledTargets.begin() + index);

        if (ScheduledTargets.empty())
        {
          LoopEvent.Unsubscribe(OnLoopEvent);
        }
      }
    }

    int16_t FindEventIndex(Event<void> *eventToFind)
    {
      for (size_t index = 0; index < ScheduledTargets.size(); ++index)
      {
        shared_ptr<ScheduledEvent> scheduledEvent = ScheduledTargets[index];

        if (scheduledEvent->TargetEvent == eventToFind)
          return index;
      }

      return -1;
    }

    void OnLoopEvent(void *args)
    {
      vector<Event<void> *> TargetsToRemove;
      timespan_t presentTime = GetUptime_us();

      for (size_t index = 0; index < ScheduledTargets.size(); ++index)
      {
        shared_ptr<ScheduledEvent> scheduledEvent = ScheduledTargets[index];

        if (presentTime - scheduledEvent->LastExecution_us > scheduledEvent->Interval_us)
        {
          scheduledEvent->TargetEvent->Invoke(nullptr);

          
          if (scheduledEvent->Mode == SchedulingBehaviour::OneShot)
          {
            TargetsToRemove.push_back(scheduledEvent->TargetEvent);
          }

          else if (scheduledEvent->Mode == SchedulingBehaviour::FixedPeriod)
          {
            scheduledEvent->LastExecution_us = scheduledEvent->LastExecution_us + scheduledEvent->Interval_us;
          }

          else if (scheduledEvent->Mode == SchedulingBehaviour::FixedPeriodSkipTicks)
          {
            timespan_t uptime = GetUptime_us();

            do
            {
              scheduledEvent->LastExecution_us = scheduledEvent->LastExecution_us + scheduledEvent->Interval_us;
            }
            while (scheduledEvent->LastExecution_us <= uptime);
          }

          else if (scheduledEvent->Mode == SchedulingBehaviour::FixedPause)
          {
            scheduledEvent->LastExecution_us = GetUptime_us();
          }
        }
      }

      for (size_t index = 0; index < TargetsToRemove.size(); ++index)
      {
        RemoveEvent(TargetsToRemove[index]);
      }
    }

  } // namespace System
} // namespace Services
