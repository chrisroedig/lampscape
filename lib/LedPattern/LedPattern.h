#ifndef LedPattern_h
#define LedPattern_h

#define MAX_COLORS 4
#include <Adafruit_NeoPixel.h>


class LedPattern
{
public:

    LedPattern(int num_pixels);
    LedPattern();
    ~LedPattern();
    int _num_pixels;
    int ** _colors;
    int _num_colors;
    int _grad_steps;
    double _current_mix;

    void set_colors(int num_colors, int color[][4]);
    void set_colors(int num_colors, int **color);
    int* pixel(int index);
    void assign(Adafruit_NeoPixel * strip);
    void assign(Adafruit_NeoPixel * strip, int start_pixel, int end_pixel);
    void init(int num_pixels);
private:


    int* fixed_color(double mix);
    int* calc_gradient_color(double gradient_pos);
    int* calc_mixed_color(int a_color[4], int b_color[4], double mix);
    void set_pixel(Adafruit_NeoPixel * strip, int index, int color[4]);
};

class LedBlink : public  LedPattern
{

public:
  LedBlink(int num_pixels);
  LedBlink();

  int _num_steps;
  double *_steps;
  int *_times;
  int _period;

  void set_blink(int num_steps, int times[], double steps[]);
  void tick(int time_ms);
};
#endif
