#include <LedWaveGauge.h>

LedWaveGauge::LedWaveGauge(){
  _colors[0] = 0;
  _colors[1] = 0;
  _colors[2] = 0;
  _marker0 = 0;
  _marker1 = 0;
  _freqs[0] = 0;
  _freqs[1] = 0;
  _freqs[2] = 0;
  _speed = 1.0/15.0;
  _period = 0.5;
}
LedWaveGauge::~LedWaveGauge(){

}
uint32 LedWaveGauge::pixel(int i, int numPixels){
  int thresh0 = (int)((float)_marker0)/255.0 * numPixels;
  int thresh1 = (int)((float)_marker1)/255.0 * numPixels;

  if( i < thresh0){
    return color(0,i);
  }else if( (i >= thresh0) && (i < thresh1)){
    return color(1,i);
  }else if( i >= thresh1){
   return color(2,i);
  }else{
    return 0;
  }
  
}
void LedWaveGauge::setPalette(int32 colors[4]){
  _colors[0] = colors[0];
  _colors[1] = colors[1];
  _colors[2] = colors[2];
  Serial.printf("params %i, %i,%i",
        _colors[0],_colors[1],_colors[2]
      );
}
void LedWaveGauge::setParams(int params[16]){
  _marker0 = params[0];
  _marker1 = params[1];
  _freqs[0] = params[2];
  _freqs[1] = params[3];
  _freqs[2] = params[4];
  _mods[0] = params[5];
  _mods[1] = params[6];
  _mods[2] = params[7];
}

uint32 LedWaveGauge::color(int region, int pixel){
  float time = millis() / 1000.0;
  
  float phase =  (float)(time *_speed* _freqs[region]+ ((float)pixel*_period));
  uint8 amp = 255 - round(_mods[region]*(0.5 + (0.5 * sin(phase))));
  return dimmedColor(_colors[region], amp);
}

uint32 dimmedColor(uint32 c, uint8 amp){
  int w = (((uint8_t)(c >> 24))*amp)>>8;
  int r = (((uint8_t)(c >> 16))*amp)>>8;
  int g = (((uint8_t)(c >>  8))*amp)>>8;
  int b = (((uint8_t)c)*amp)>>8;
  return (w<<24)|(r<<16)|(g<<8)|(b);
}