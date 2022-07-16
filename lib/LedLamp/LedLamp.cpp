#include <LedLamp.h>

LedLamp::LedLamp(){
  _color = 0;
}
LedLamp::~LedLamp(){

}
uint32 LedLamp::pixel(int i, int numPixels){
  return _color;
}

void LedLamp::tick(){

}

void LedLamp::setPalette(int32 colors[4]){
  _color = colors[0];
}
void LedLamp::setParams(int params[4]){

}

uint32 dimmedColor(uint32 c, uint8 amp){
  int w = (((uint8_t)(c >> 24))*amp)>>8;
  int r = (((uint8_t)(c >> 16))*amp)>>8;
  int g = (((uint8_t)(c >>  8))*amp)>>8;
  int b = (((uint8_t)c)*amp)>>8;
  return (w<<24)|(r<<16)|(g<<8)|(b);
}

uint32 mixedColor(uint32 c1, uint32 c2, uint8 mix){
  int w1 = (uint8_t)(c1 >> 24);
  int r1 = (uint8_t)(c1 >> 16);
  int g1 = (uint8_t)(c1 >>  8);
  int b1 = (uint8_t)c1;
  
  int w2 = (uint8_t)(c2 >> 24);
  int r2 = (uint8_t)(c2 >> 16);
  int g2 = (uint8_t)(c2 >>  8);
  int b2 = (uint8_t)c2;
  
  int w = (w1*(255-mix) + w2*mix)>>8;
  int r = (r1*(255-mix) + r2*mix)>>8;
  int g = (g1*(255-mix) + g2*mix)>>8;
  int b = (b1*(255-mix) + b2*mix)>>8;

  return (w<<24)|(r<<16)|(g<<8)|(b);
}