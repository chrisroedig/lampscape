#include "LedPattern.h"
#include <Adafruit_NeoPixel.h>

LedPattern::LedPattern(int num_pixels){ init(num_pixels); };

LedPattern::LedPattern(){ init(32);};

LedPattern::~LedPattern(){
  delete [] _colors;
};

void LedPattern::init(int num_pixels){
    _num_pixels = num_pixels;
    _num_colors = 4;
    _current_mix = 0.0;
    int colors[2][4] = {{0,0,0,0},{255,255,255,255}};
    set_colors(2, colors);
};

void LedPattern::set_colors(int num_colors, int color[][4]){
  _num_colors = num_colors;
  _colors = new int*[_num_colors]; // pointer to array for each color
  for(int i = 0; i < _num_colors; i++ ){ //each color
    _colors[i] = new int[4]; // channel array
    for(int j=0; j< 4;j++){ //each channel
      _colors[i][j]=color[i][j];
    }
  }
  //build_color_cache();
};

int* LedPattern::pixel(int index){
  // the pattern color scheme at _current_mix
  return fixed_color(_current_mix);
};

void LedPattern::assign(Adafruit_NeoPixel * strip){
  // assign all the pixels
  assign(strip, 0, strip->numPixels());
};

void LedPattern::assign(Adafruit_NeoPixel * strip, int start_pixel, int end_pixel){
  // assign a range of pixels
  for(int i=start_pixel; i < end_pixel; i++){
    set_pixel(strip, i, pixel(i));
  }
};

void LedPattern::set_pixel(Adafruit_NeoPixel * strip, int index, int color[4]){
  // set the pixel to the strip
  strip->setPixelColor(index,strip->Color(color[0], color[1], color[2], color[3]));
};

int* LedPattern::fixed_color(double mix){
  // color, given the mix
  // check cache first, maybe
  return calc_gradient_color(mix);
};

int* LedPattern::calc_gradient_color(double grad_pos){
  // calculate color along gradient of _colors
  int a_color_index = max(0,(int) floor(grad_pos*_num_colors));
  int b_color_index = min(_num_colors-1,(int) ceil(grad_pos*_num_colors));
  double mix = _num_colors * grad_pos - floor(_num_colors * grad_pos);
  return calc_mixed_color(_colors[a_color_index], _colors[b_color_index], mix);
};

int* LedPattern::calc_mixed_color(int a_color[4], int b_color[4], double mix){
  // return weighted, mixed color
  static int mixed_color[4] = {0,0,0,0};
  if(mix > 1.0){ mix = 1.0;};
  if(mix < 0.0){ mix = 0.0;};
  for(int i = 0 ; i<4; i++){
    mixed_color[i] = (int)round(a_color[i]*(1.0-mix) + b_color[i]* mix);
  }
  return mixed_color;
};

LedBlink::LedBlink(int num_pixels) : LedPattern(num_pixels){ init(num_pixels); };
LedBlink::LedBlink() : LedPattern(){};
//
// LedBlink::LedBlink(){ init(32);};
//


void LedBlink::set_blink(int num_steps, int times[], double steps[]){
  _num_steps = num_steps;
  _steps = new double[_num_steps];
  _times = new int[_num_steps];
  _period = 0;
  for(int i =0 ; i < _num_steps; i++){
      _steps[i] = steps[i];
      _times[i] = times[i];
      _period += _times[i];
  }
}

void LedBlink::tick(int time_ms){
  int mod_time = time_ms % _period;
  double pos_mix =0;
  int pos=0;
  for (int i = 0; i < _num_steps-1; i++) {
    if( _times[i] < mod_time &&  mod_time < _times[i+1] ){
        pos_mix = (double)(mod_time - _times[i])/(double)(_times[i+1]-_times[i]);
        pos = i;
    }
  }
  _current_mix = 0.5*(_steps[pos]*(1.0-pos_mix) + _steps[pos+1]*pos_mix);
}
