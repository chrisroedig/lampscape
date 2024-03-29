#ifndef LedLamp_h
#define LedLamp_h

#include <Arduino.h>

class LedLamp{

public:
  LedLamp();
  ~LedLamp();
  virtual uint32 pixel(int, int);
  virtual void tick();
  void setPalette(int32[4]);
  void setParams(int[4]);
  
private:
  uint32 _color;

};
uint32 dimmedColor(uint32 c, uint8 amp);
uint32 mixedColor(uint32 c1, uint32 c2, uint8 mix);

#endif