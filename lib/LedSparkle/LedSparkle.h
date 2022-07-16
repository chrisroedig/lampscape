#ifndef LedSparkle_h
#define LedSparkle_h

#include <Arduino.h>
#include <LedLamp.h>
#include <math.h>

class LedSparkle : public LedLamp{

  public:
  LedSparkle();
  ~LedSparkle();
  uint32 pixel(int, int);
  void tick();
  void setPalette(int32[4]);
  void setParams(int[16]);
  uint32 color(int, int);

  private:
  void newSpark();
  void updateSparks();
  uint32 _colors[4] = {0,0,0,0};
  uint8 _sparkTimes[256];
  uint8 _sparkDecay[256];
  uint8 _sparkLocation[256];
  uint8 _ledAmps[256];
  uint8 _sparkRate = 10;
  uint8 _sparkDecayRate = 10;
  uint8 _sparkRateMultiplier = 10;
  int _sparkCount;
  uint8 _sparkPointer=0;
  uint8 _profileExp[256];

};

#endif