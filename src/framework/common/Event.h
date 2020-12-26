#ifndef _Event_
#define _Event_

#include <Arduino.h>
#include <vector>

using namespace std;

template <class T>
class Event
{

private:

  vector<void (*)(T*)> ExecutionTargets;

public:

  void Subscribe(void (*function)(T*))
  {
    if (!(find(ExecutionTargets.begin(), ExecutionTargets.end(), function) != ExecutionTargets.end()))
    {
      ExecutionTargets.push_back(function);
    }
  }

  void Unsubscribe(void (*function)(T*))
  {
    if (find(ExecutionTargets.begin(), ExecutionTargets.end(), function) != ExecutionTargets.end())
    {
      ExecutionTargets.erase(remove(ExecutionTargets.begin(), ExecutionTargets.end(), function), ExecutionTargets.end());
    }
  }

  void Invoke(T *handler)
  {
    vector<void (*)(T*)> copyOfExecutionTargets = ExecutionTargets;

    for (auto target : copyOfExecutionTargets)
    {
      target(handler);
    }
  }
};

#endif
