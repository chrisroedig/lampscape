#ifndef MqttClient_h
#define MqttClient_h

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MqttClient{

public:
  
  MqttClient();
  ~MqttClient();
  bool setup(char* server, char* mqtt_port, char* device_id);
  void loop();
  void publish(char* topic, char* message);
  void set_power_callback(void (*func)(bool) );
  void set_bright_callback(void (*func)(int) );

private:

  PubSubClient _client;
  char * _device_id;
  void (*_cmd_power_callback)(bool) = NULL;
  void (*_cmd_bright_callback)(int) = NULL;
  void inbound_callback(char* topic, byte* payload, unsigned int length);

};
#endif
