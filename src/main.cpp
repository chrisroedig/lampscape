#include "main.h"


WiFiManager wifiManager;
MqttClient mclient;
Ticker ticker;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, 4, NEO_GRBW + NEO_KHZ800);

LedLamp ledLamp = LedLamp();
LedLamp ledOff = LedLamp();
LedWaveGauge ledWave = LedWaveGauge();

int ledBrightness = 255;
bool ledActive = false;

int ledMode = MODE_LAMP;
uint32 ledLampColor = 0;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  wifiInit();
  ledsInit(32);
  mqttInit("rileypi.local", "lampscape2");

}

void loop() {
  // put your main code here, to run repeatedly:
  mclient.loop();
  ledLoop();
  delay(10);
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
  mclient.set_power_callback(*mqttPwrCallback);
  mclient.set_bright_callback(*mqttBrightCallback);
  mclient.set_mode_callback(*mqttModeCallback);
  mclient.set_params_callback(*mqttParamsCallback);
  mclient.set_palette_callback(*mqttPaletteCallback);
}


void ledLoop(){
  strip.setBrightness(ledBrightness);
  for(int i=0;i<NUM_LEDS; i++){
    strip.setPixelColor(i, driver->pixel(i, NUM_LEDS));
  }
  strip.show();
}

void mqttPwrCallback(bool state){
  ledActive = state;
}

void mqttBrightCallback(int brightness){
  ledBrightness = brightness;
  Serial.printf("LED bright is now %i\n", ledBrightness);
}

void mqttModeCallback(int mode){
  ledMode = mode;
  Serial.printf("LED mode is now %i\n", ledMode);
  switch (mode)
  {
  case MODE_OFF:
    driver = &ledOff;
    break;
  case MODE_LAMP:
    driver = &ledLamp;
    break;
  case MODE_WAVEGAUGE:
    driver = &ledWave;
    break;
  default:
    driver = &ledOff;
    break;
  }
}

void mqttPaletteCallback(int32 palette[4]){
  ledLamp.setPalette(palette);
  ledWave.setPalette(palette);
}

void mqttParamsCallback(int params[8]){
  ledLamp.setParams(params);
  ledWave.setParams(params);
}


