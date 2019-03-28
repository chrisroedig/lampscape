#include <FS.h>
#include <WiFiManager.h>
//for LED status
#include <Ticker.h>
#include <MqttClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <LedPattern.h>

#define NUM_LEDS 32
char* mqtt_server = "rileypi.local";
char* device_name = "lampscape1";

WiFiManager wifiManager;
MqttClient mclient;

Ticker ticker;

LedPattern * patterns;
int pattern_count = 0;
String current_layout_name = "";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, 4, NEO_GRBW + NEO_KHZ800);
int led_brightness = 100;
bool led_active = true;
void tick(){
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
}

void configModeCallback(WiFiManager *myWiFiManager){
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  ticker.attach(0.2, tick);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }
  mclient.setup(mqtt_server, device_name);
  mclient.set_power_callback(set_power);
  mclient.set_bright_callback(set_brightness);
  mclient.set_layout_callback(set_layout);
  mclient.set_values_callback(set_values);

  update_brightness();
  strip.begin();
  strip.show();
}

void update_brightness(){
  if(led_active){
    strip.setBrightness(led_brightness);
  }else{
    strip.setBrightness(20);
  }
}
void set_power(bool state){
  Serial.printf("POWER -> %i\n", state);
  led_active = state;
  update_brightness();

  mclient.publish_state("POWER", state?"ON":"OFF" );
}

void set_brightness(int brightness){
  Serial.printf("BRIGHTNESS -> %i\n", brightness);
  if(brightness > 255){
    return;
  }
  led_brightness = brightness;
  update_brightness();
  mclient.publish_state("BRIGHTNESS", String(led_brightness));
}
void set_values(String payload){
  mclient.log("layout values command");
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  String layout_name = root["layout"];
  if(layout_name != current_layout_name){
    mclient.log("layout values: wrong layout");
    return;
  }
  JsonArray& pattern_json_array = root["patterns"];
  if(pattern_json_array.size() < pattern_count){
    mclient.log("layout values: pattern mismatch");
    return;
  }
  for(int i=0; i<pattern_count; i++){
      JsonObject& pattern_params = pattern_json_array[i];
      configure_pattern(patterns[i], pattern_params);
  }
  strip.show();
}
void configure_pattern(LedPattern &pattern, JsonObject &params){
  int start_pixel = params["start"];
  int end_pixel = params["end"];
  pattern.assign(&strip, start_pixel, end_pixel);
}
void set_layout(String layout_url){
  HTTPClient http;
  mclient.log(String("fecthing layout: ")+layout_url);
  if(!http.begin(layout_url)){
    return;
  }
  Serial.printf("[HTTP] GET %s \n", layout_url.c_str());
  int httpCode = http.GET();
  if (httpCode < 0) {
    mclient.log("failed to connect to layout server");
    mclient.log("[HTTP] ERROR " +String( http.errorToString(httpCode)));
    return;
  }
  String payload = http.getString();
  http.end();
  if (httpCode != HTTP_CODE_OK ){
    mclient.log("failed to get layout");
    mclient.log("[HTTP] ERROR " +String( httpCode));
    return;
  }
  Serial.printf("[HTTP] OK\n");
  mclient.log("got layout, now processing");
  process_layout(payload);
}

void process_layout(String layout_config){
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(layout_config);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  JsonArray& pattern_json_array = root["patterns"];
  String layout_name = root["name"];
  current_layout_name = layout_name;
  Serial.printf("Patterns: %i \n", pattern_json_array.size());
  pattern_count = pattern_json_array.size();
  patterns = new LedPattern[pattern_count];

  mclient.log("setting up layout: "+ current_layout_name);
  for(int i=0; i<pattern_json_array.size(); i++){
      JsonObject& pattern_data = pattern_json_array[i];
      String ptype = pattern_data["type"];

      if(ptype == "blink"){
        set_solid_pattern(patterns[i], pattern_data);
      }else{
        set_blink_pattern(patterns[i], pattern_data);
      }
  }
  assign_patterns();
  mclient.log(String("layout processed"));
}
void set_solid_pattern(LedPattern &pattern, JsonObject &pattern_data){
  pattern = LedBlink();
  set_pattern_colors(pattern, pattern_data);
}
void set_blink_pattern(LedPattern &pattern, JsonObject &pattern_data){
  pattern = LedPattern();
  set_pattern_colors(pattern, pattern_data);
}
void set_pattern_colors(LedPattern &pattern, JsonObject &pattern_data){
  JsonArray& color_json_array = pattern_data["colors"];
  int num_colors = color_json_array.size();
  int **colors = new int*[num_colors];

  for(int j = 0; j < num_colors; j++ ){
    colors[j] = new int[4];
    for(int k=0; k< 4;k++){
      colors[j][k]=color_json_array[j][k];
    }
  }
  pattern.set_colors(num_colors, colors);
}

void assign_patterns(){
  //patterns[0].assign(&strip);
  //strip.show();
}

void loop() {
  mclient.loop();
}
