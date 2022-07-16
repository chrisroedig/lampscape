#ifndef LedWaveGauge_h
#define LedWaveGauge_h

#include <Arduino.h>
#include <LedLamp.h>

class LedWaveGauge : public LedLamp{

public:
  LedWaveGauge();
  ~LedWaveGauge();
  uint32 pixel(int, int);
  
  void setPalette(int32[4]);
  void setParams(int[16]);
  uint32 color(int, int);

private:
  int _marker0;
  int _marker1;
  float _speed;
  float _pitch;
  uint32 _colors[3] = {0,0,0};
  int _freqs[3] = {0,0,0};
  int _sizes[3] = {127,127,127};
  int _amps[3] = {255,255,255};

};

uint32 dimmedColor(uint32 c, uint8 amp);



#endif