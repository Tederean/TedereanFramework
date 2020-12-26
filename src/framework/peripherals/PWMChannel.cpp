#if defined(ESP8266) || defined(ESP32)

#include <Arduino.h>
#include <framework/utils/Math.h>
#include <framework/peripherals/PWMChannel.h>

using namespace std;

PWMChannel::PWMChannel(uint8_t pin, bool invertSignal)
{
  ChannelType = LightChannelType::DigitalPin;
  CurrentPwmValue = 0;

  Pin = pin;
  InvertSignal = invertSignal;
  Frequency_Hz = 0.0;
  Resolution_bit = 1;
  PwmMinValue = 0;
  PwmMaxValue = 1;
  LedChannel = 0;
}

PWMChannel::PWMChannel(uint8_t pin, bool invertSignal, double frequency_Hz, uint8_t resolution_bit, uint8_t ledChannel, double pwmMinRatio, double pwmMaxRatio)
{
  ChannelType = LightChannelType::ESP32PWM;
  CurrentPwmValue = 0;

  Pin = pin;
  InvertSignal = invertSignal;
  Frequency_Hz = frequency_Hz;
  Resolution_bit = resolution_bit;
  PwmMinValue = PwmRatioToValue(resolution_bit, pwmMinRatio);
  PwmMaxValue = PwmRatioToValue(resolution_bit, pwmMaxRatio);
  LedChannel = ledChannel;
}

PWMChannel::PWMChannel(uint8_t pin, bool invertSignal, double frequency_Hz, uint8_t resolution_bit, double pwmMinRatio, double pwmMaxRatio)
{
  ChannelType = LightChannelType::ESP8266PWM;
  CurrentPwmValue = 0;

  Pin = pin;
  InvertSignal = invertSignal;
  Frequency_Hz = frequency_Hz;
  Resolution_bit = resolution_bit;
  PwmMinValue = PwmRatioToValue(resolution_bit, pwmMinRatio);
  PwmMaxValue = PwmRatioToValue(resolution_bit, pwmMaxRatio);
  LedChannel = 0;
}

void PWMChannel::Initialize()
{
  if (ChannelType == LightChannelType::DigitalPin)
  {
    pinMode(Pin, OUTPUT);
  }

#if defined(ESP32)
  else if (ChannelType == LightChannelType::ESP32PWM)
  {
    ledcSetup(LedChannel, Frequency_Hz, Resolution_bit);
    ledcAttachPin(Pin, LedChannel);
  }
#endif

#if defined(ESP8266)
  else if (ChannelType == LightChannelType::ESP8266PWM)
  {
    uint16_t resolution_counts = ResolutionToCounts(Resolution_bit);

    analogWriteFreq(Frequency_Hz);
    analogWriteRange(resolution_counts);
    pinMode(Pin, OUTPUT);
  }
#endif

  WriteCounts(PwmMinValue, true);
}

void PWMChannel::WriteRatio(double ratio, bool force)
{
  uint16_t counts = round(Math::Map<double>(ratio, 0.0, 1.0, PwmMinValue, PwmMaxValue));

  WriteCounts(counts, force);
}

void PWMChannel::WriteCounts(uint16_t value, bool force)
{
  value = Math::Clamp<uint16_t>(value, PwmMinValue, PwmMaxValue);

  if (InvertSignal)
  {
    value = ~value;
    value = value << (16 - Resolution_bit);
    value = value >> (16 - Resolution_bit);
  }

  if (!force && value == CurrentPwmValue)
    return;

  CurrentPwmValue = value;

  if (ChannelType == LightChannelType::DigitalPin)
  {
    digitalWrite(Pin, value);
  }

#if defined(ESP32)
  else if (ChannelType == LightChannelType::ESP32PWM)
  {
    ledcWrite(LedChannel, value);
  }
#endif

#if defined(ESP8266)
  else if (ChannelType == LightChannelType::ESP8266PWM)
  {
    analogWrite(Pin, value);
  }
#endif
}

uint16_t PWMChannel::PwmRatioToValue(uint8_t resolution_bit, double ratio)
{
  uint16_t resolution_counts = ResolutionToCounts(resolution_bit);

  uint16_t value = round(resolution_counts * ratio);

  return Math::Clamp<uint16_t>(value, 0, resolution_counts);
}

uint16_t PWMChannel::ResolutionToCounts(uint8_t resolution_bit)
{
  return pow(2, resolution_bit) - 1;
}

#endif