#include <LedSparkle.h>

LedSparkle::LedSparkle(){
  for(int i=0; i<256; i++){
    _sparkTimes[i]=0;
    _ledAmps[i]=0;
    _profileExp[i] = round(255*(1.0-exp(-10.0/((float)(256-i)))));
  }
  _profileExp[0] = 0;
}
LedSparkle::~LedSparkle(){

}

uint32 LedSparkle::pixel(int i, int numPixels){
  int led_i = round(255.0 * (float)i / (float)numPixels);
  return mixedColor(_colors[0], _colors[1], _ledAmps[led_i]);
  //return dimmedColor(_colors[0], _ledAmps[led_i]);
}

void LedSparkle::tick(){
  for(int i=0; i<_sparkRateMultiplier; i++){
    if(random(256) < _sparkRate){
      newSpark();
    }
  }
  updateSparks();
}

void LedSparkle::newSpark(){
  
  int pIndex = (_sparkPointer++)%256;
  _sparkTimes[pIndex] = 255;
  _sparkDecay[pIndex] = _sparkDecayRate;
  _sparkLocation[pIndex] = random(256);
  //Serial.printf("spark %i, %i, %i \n", pIndex, _sparkDecay[pIndex], _sparkLocation[pIndex]);
}

void LedSparkle::updateSparks(){
  for(int i=0; i<256; i++){
    float delta = (float)_sparkDecay[i]/10.0;
    _sparkTimes[i] = (_sparkTimes[i]>delta) *  (_sparkTimes[i]-delta);
    _ledAmps[_sparkLocation[i]] = _profileExp[_sparkTimes[i]];
  }
}

void LedSparkle::setPalette(int32 colors[4]){
  _colors[0] = colors[0];
  _colors[1] = colors[1];
  Serial.printf("palette %i, %i,%i",
        _colors[0],_colors[1],_colors[2]
      );
}
void LedSparkle::setParams(int params[4]){
  _sparkRate = params[0];
}