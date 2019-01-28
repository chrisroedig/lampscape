#include <MqttClient.h>

// PubSubClient _client;
// char * _device_id;
// void (*_cmd_power_callback)(bool) = NULL;
// void (*_cmd_bright_callback)(int) = NULL;

MqttClient::MqttClient(){};
MqttClient::~MqttClient(){};

bool MqttClient::setup(char* server, char* mqtt_port, char* device_id){
  // WiFi.status() == WL_CONNECTED ???
  // client.setServer(mqtt_server, 1883);
  // client.setCallback(inbound_callback);
  return false;
};

void MqttClient::loop(){
// if (!client.connected()) {
//   reconnect();
// }
// client.loop();
};
void MqttClient::publish(char* topic, char* message){
// mqtt_topic = snprintf( device_id/topic )
// client.publish(mqtt_topic, msg);
};
void MqttClient::set_power_callback(void (*func)(bool) ){
};
void MqttClient::set_bright_callback(void (*func)(int) ){
// _cmd_bright_callback = func;
};
void MqttClient::inbound_callback(char* topic, byte* payload, unsigned int length){
// for i in range length -> (char)payload[i]
};
