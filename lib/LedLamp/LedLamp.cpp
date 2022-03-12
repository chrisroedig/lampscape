#include <LedLamp.h>

LedLamp::LedLamp(){
  _color = 0;
}
LedLamp::~LedLamp(){

}
uint32 LedLamp::pixel(int i, int numPixels){
  return _color;
}

void LedLamp::setPalette(int32 colors[4]){
  _color = colors[0];
}
void LedLamp::setParams(int params[4]){

}