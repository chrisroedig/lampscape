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
  _pitch = 64.0;
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
  Serial.printf("palette %i, %i,%i",
        _colors[0],_colors[1],_colors[2]
      );
}
void LedWaveGauge::setParams(int params[16]){
  _marker0 = params[0];
  _marker1 = params[1];
  _freqs[0] = params[2];
  _freqs[1] = params[3];
  _freqs[2] = params[4];
  _sizes[0] = params[5];
  _sizes[1] = params[6];
  _sizes[2] = params[7];
  _amps[0] = params[8];
  _amps[1] = params[9];
  _amps[2] = params[10];
  Serial.printf("speeds: %i %i %i \n", _freqs[0], _freqs[1], _freqs[2]);
  Serial.printf("sizes: %i %i %i \n", _sizes[0], _sizes[1], _sizes[2]);
  Serial.printf("amps: %i %i %i \n", _amps[0], _amps[1], _amps[2]);
}

uint32 LedWaveGauge::color(int region, int pixel){
  float time = millis() / 1000.0;
  
  float phase = 0;
  phase += (float)(time *_speed* _freqs[region]);
  phase += ((float)pixel*_pitch)/_sizes[region];
  int amp = 255 - round(_amps[region]*(0.5 + (0.5 * sin(phase))));
  // amp = amp * amp / 65536;
  return dimmedColor(_colors[region], amp);
}

