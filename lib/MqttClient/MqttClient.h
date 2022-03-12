#ifndef MqttClient_h
#define MqttClient_h

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

class MqttClient{

public:

  MqttClient();
  ~MqttClient();
  bool setup(char * mqtt_server, char *  device_name);
  void loop();
  void publish(String topic, String message);
  void publish_state(String topic, String message);
  void log(String message);
  void announce_boot();

  void set_bright_callback(void (*func)(int) );
  void set_mode_callback(void (*func)(int) );
  void set_palette_callback(void (*func)(int32[4]) );
  void set_params_callback(void (*func)(int[16]) );

private:
  PubSubClient _client;
  WiFiClient _wifi_client;
  String _device_name;
  String _power_topic;
  String _bright_topic;
  String _mode_topic;
  String _palette_topic;
  String _params_topic;
  
  void (*_cmd_bright_callback)(int) = NULL;
  void (*_cmd_mode_callback)(int) = NULL;
  void (*_cmd_palette_callback)(int32[4]) = NULL;
  void (*_cmd_params_callback)(int[8]) = NULL;
  
  void subscribe();
  void reconnect();
  void inbound_callback(char* topic, uint8_t* payload, unsigned int length);
};

#endif