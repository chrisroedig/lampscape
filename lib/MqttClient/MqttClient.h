#ifndef MqttClient_h
#define MqttClient_h


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MqttClient{

public:

  MqttClient();
  ~MqttClient();
  bool setup(char * server, char * device_name);
  void loop();
  void publish(String topic, String message);
  void publish_state(String topic, String message);
  void log(String message);
  void announce_boot();
  void set_power_callback(void (*func)(bool) );
  void set_bright_callback(void (*func)(int) );
  void set_layout_callback(void (*func)(String) );
  void set_values_callback(void (*func)(String) );

private:
  PubSubClient _client;
  WiFiClient _wifi_client;
  char * _device_name;
  String _power_topic;
  String _bright_topic;
  String _layout_topic;
  String _values_topic;
  void (*_cmd_power_callback)(bool) = NULL;
  void (*_cmd_bright_callback)(int) = NULL;
  void (*_cmd_layout_callback)(String) = NULL;
  void (*_cmd_values_callback)(String) = NULL;
  void subscribe();
  void reconnect();
  void inbound_callback(char* topic, uint8_t* payload, unsigned int length);
};
#endif
