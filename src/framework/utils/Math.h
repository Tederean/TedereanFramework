#ifndef _Math_
#define _Math_

#include <Arduino.h>

using namespace std;

namespace Math
{

  template <typename T>
  T Clamp(T value, T min, T max)
  {
    if (value > max)
      return max;

    if (value < min)
      return min;

    return value;
  }

  template <typename T>
  T Map(T in_value, T in_min, T in_max, T out_min, T out_max)
  {
    return (in_value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

} // namespace Math

#endif
