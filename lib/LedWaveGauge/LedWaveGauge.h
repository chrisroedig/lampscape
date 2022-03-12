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
  void setParams(int[4]);
  uint32 color(int, int);

private:
  int _marker0;
  int _marker1;
  float _speed;
  float _period;
  uint32 _colors[3] = {0,0,0};
  int _freqs[3] = {0,0,0};
};

uint32 dimmedColor(uint32 c, uint8 amp);



#endif