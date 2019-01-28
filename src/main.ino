#include <FS.h>

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <PubSubClient.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <PubSubClient.h>

//for LED status
#include <Ticker.h>

#include <Adafruit_NeoPixel.h>
#include <LedPattern.h>

Ticker ticker;
Ticker led_ticker;
#define NUM_LEDS 32
#define BRIGHTNESS 100

#define TRIGGER_PIN 14

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, 4, NEO_GRBW + NEO_KHZ800);
LedPattern solid_pattern = LedPattern(NUM_LEDS);
LedBlink blink_pattern = LedBlink(NUM_LEDS);
//define your default values here, if there are different values in config.json, they are overwritten.
char mqtt_server[40];
char mqtt_port[6] = "8080";

void tick(){
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
}

void update_leds(){
  blink_pattern.tick(millis());

  blink_pattern.assign(&strip,15, 17);
  strip.show();
}

void configModeCallback(WiFiManager *myWiFiManager){
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  ticker.attach(0.2, tick);
}

WiFiManager wifiManager;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT);
  ticker.attach(0.6, tick);

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);

  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  Serial.println(millis());
  ticker.detach();
  //keep LED on
  digitalWrite(LED_BUILTIN, LOW);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  int pattern_colors[4][4] = {
    {255,0,0,0},
    {0,255,0,50},
    {0,50,255,50},
    {0,0,255,100}
  };
  int blink_colors[2][4] = {
    {0,0,0,10},
    {0,100,100,50}
  };
  double blink_steps[6] = { 0.0, 1.0, 0.0, 0.0 };
  int blink_times[4]    = { 0,   1000,  1000, 1000 };
  solid_pattern.set_colors(4, pattern_colors);
  blink_pattern.set_colors(2, blink_colors );
  blink_pattern.set_blink(4, blink_times, blink_steps);

  solid_pattern.assign(&strip);
  strip.show();
  ticker.attach(0.02, update_leds);
}

void loop() {
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    if (!wifiManager.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
    Serial.println("config done!");
  }

}
