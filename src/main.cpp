#include "main.h"
#include <config.h>

WiFiManager wifiManager;
MqttClient mclient;
Ticker ticker;
Adafruit_NeoPixel strip = Adafruit_NeoPixel( NEO_PIXELS, NEO_PIN, NEO_INTERFACE);

LedLamp ledLamp = LedLamp();
LedLamp ledOff = LedLamp();
LedWaveGauge ledWave = LedWaveGauge();
LedSparkle ledSparkle = LedSparkle();

int ledBrightness = 255;
int ledMode = MODE_LAMP;
uint32 ledLampColor = 0;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  wifiInit();
  ledsInit(32);
  mqttInit(MQTT_HOST, DEVICE_NAME);

}

void loop() {
  // put your main code here, to run repeatedly:
  mclient.loop();
  ledLoop();
  delay(5);
}

void tick(){
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
}


void wifiConfigModeCallback(WiFiManager *myWiFiManager){
  Serial.println("WIFI config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  ticker.attach(0.5, tick);
}

void wifiInit(){
  wifiManager.setAPCallback(wifiConfigModeCallback);
  Serial.println("WIFI connecting");
  ticker.attach(0.1, tick);
  if (!wifiManager.autoConnect()) {
    
    Serial.println("WIFI no connect, resetting");
    ESP.reset();
    delay(1000);
  }
  Serial.println("WIFI connected");
  ticker.detach();
}

void ledsInit(int count){
  Serial.println("setting up LEDs");
  strip.begin();
  driver = &ledOff;
  ledLoop();
}

void mqttInit(char * hostname, char * deviceName){
  Serial.println("setting up MQTT");
  mclient.setup(hostname,  deviceName);
  mclient.set_bright_callback(*mqttBrightCallback);
  mclient.set_mode_callback(*mqttModeCallback);
  mclient.set_params_callback(*mqttParamsCallback);
  mclient.set_palette_callback(*mqttPaletteCallback);
}


void ledLoop(){
  driver->tick();
  strip.setBrightness(ledBrightness);
  for(int i=0;i<NEO_PIXELS; i++){
    strip.setPixelColor(i, driver->pixel(i, NEO_PIXELS));
  }
  strip.show();
}

void mqttBrightCallback(int brightness){
  ledBrightness = brightness;
  Serial.printf("LED bright is now %i\n", ledBrightness);
}

void mqttModeCallback(int mode){
  ledMode = mode;
  Serial.printf("LED mode is now %i ", ledMode);
  switch (mode)
  {
  case MODE_OFF:
    Serial.printf("OFF\n");
    driver = &ledOff;
    break;
  case MODE_LAMP:
    Serial.printf("LAMP\n");
    driver = &ledLamp;
    break;
  case MODE_WAVEGAUGE:
    Serial.printf("WAVE GAUGE\n");
    driver = &ledWave;
    break;
  case MODE_SPARKLE:
    Serial.printf("SPARKLE\n");
    driver = &ledSparkle;
    break;
  default:
    Serial.printf("DEFAULT OFF\n");
    driver = &ledOff;
    break;
  }
}

void mqttPaletteCallback(int32 palette[4]){
  ledLamp.setPalette(palette);
  ledWave.setPalette(palette);
  ledSparkle.setPalette(palette);
}

void mqttParamsCallback(int params[16]){
  ledLamp.setParams(params);
  ledWave.setParams(params);
  ledSparkle.setParams(params);
}


