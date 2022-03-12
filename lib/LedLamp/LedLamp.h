#ifndef LedLamp_h
#define LedLamp_h

#include <Arduino.h>

class LedLamp{

public:
  LedLamp();
  ~LedLamp();
  virtual uint32 pixel(int, int);
  void setPalette(int32[4]);
  void setParams(int[4]);
  
private:
  uint32 _color;

};



#endif