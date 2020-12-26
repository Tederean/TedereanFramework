#if !defined(_PWMChannel_) && (defined(ESP8266) || defined(ESP32))
#define _PWMChannel_

#include <Arduino.h>

using namespace std;

enum class LightChannelType
{
  DigitalPin,
  ESP8266PWM,
  ESP32PWM,
};

class PWMChannel
{

private:

  LightChannelType ChannelType;

  uint8_t Pin;

  bool InvertSignal;

  double Frequency_Hz;

  uint8_t Resolution_bit;

  uint16_t PwmMinValue;

  uint16_t PwmMaxValue;

  uint8_t LedChannel;

  uint16_t CurrentPwmValue;
  
  uint16_t PwmRatioToValue(uint8_t resolution_bit, double ratio);

  uint16_t ResolutionToCounts(uint8_t resolution_bit);

public:

  PWMChannel(uint8_t pin, bool invertSignal);

  PWMChannel(uint8_t pin, bool invertSignal, double frequency_Hz, uint8_t resolution_bit, uint8_t ledChannel, double pwmMinRatio = 0.0, double pwmMaxRatio = 1.0);

  PWMChannel(uint8_t pin, bool invertSignal, double frequency_Hz, uint8_t resolution_bit, double pwmMinRatio = 0.0, double pwmMaxRatio = 1.0);

  void Initialize();

  void WriteCounts(uint16_t value, bool force = false);

  void WriteRatio(double ratio, bool force = false);
};

#endif