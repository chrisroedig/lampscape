#ifndef main_h
#define main_h

#define NUM_LEDS 32
#define MODE_OFF 0
#define MODE_LAMP 1
#define MODE_WAVEGAUGE 2

#include <Arduino.h>
#include <WiFiManager.h>
#include <Ticker.h>
#include <MqttClient.h>
#include <Adafruit_NeoPixel.h>
#include <LedLamp.h>
#include <LedWaveGauge.h>

void tick();
void wifiConfigModeCallback(WiFiManager *myWiFiManager);
void wifiInit();
void ledsInit(int count);
void mqttInit(char * hostname, char * deviceName);

void ledLoop();
LedLamp *driver;
void mqttPwrCallback(bool state);
void mqttBrightCallback(int brightness);
void mqttModeCallback(int mode);
void mqttPaletteCallback(int32 palette[4]);
void mqttParamsCallback(int params[8]);

#endif