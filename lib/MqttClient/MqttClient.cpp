#include <MqttClient.h>

MqttClient::MqttClient(){
  _wifi_client = WiFiClient();
  _client = PubSubClient(_wifi_client);

};

MqttClient::~MqttClient(){};

bool MqttClient::setup(char * mqtt_server, char * device_name){
  using namespace std::placeholders;
  if(WiFi.status() != WL_CONNECTED){
    return false;
  }
  _device_name = device_name;
  _client.setServer(mqtt_server, 1883);
  _client.setCallback(std::bind(&MqttClient::inbound_callback, this, _1, _2, _3));
  return true;
};

void MqttClient::loop(){
 if (!_client.connected()) {
   reconnect();
 }
 _client.loop();
};

void MqttClient::reconnect(){
  while (!_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "lampscape-";
    clientId += String(random(0xffff), HEX);

    if (_client.connect(clientId.c_str())) {
      Serial.println("connected");
      log("connected");
      subscribe();
    } else {
      Serial.print("failed, rc=");
      Serial.print(_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
};
void MqttClient::subscribe(){
  _power_topic = "cmnd/"+String(_device_name)+"/POWER";
  _bright_topic = "cmnd/"+String(_device_name)+"/BRIGHT";
  _layout_topic = "cmnd/"+String(_device_name)+"/LAYOUT";
  _values_topic = "cmnd/"+String(_device_name)+"/VALUES";
  _client.subscribe(_power_topic.c_str());
  _client.subscribe(_bright_topic.c_str());
  _client.subscribe(_layout_topic.c_str());
  _client.subscribe(_values_topic.c_str());
  log("subscribed");
  announce_boot();

}

void MqttClient::log(String message){
  String msg = "["+String(_device_name)+"]: "+String(message);
  String topic = "log/lampscape/"+String(_device_name);
  publish(topic.c_str(), msg.c_str());
}
void MqttClient::announce_boot(){
  publish("boot/lampscape", _device_name);
}
void MqttClient::publish_state(String topic, String message){
  String full_topic = "state/lampscape/"+String(_device_name);
  _client.publish(full_topic.c_str(), message.c_str());
};
void MqttClient::publish(String topic, String message){
  _client.publish(topic.c_str(), message.c_str());
};

void MqttClient::set_power_callback(void (*func)(bool) ){
  _cmd_power_callback = func;
};
void MqttClient::set_bright_callback(void (*func)(int) ){
  _cmd_bright_callback = func;
};
void MqttClient::set_layout_callback(void (*func)(String) ){
  _cmd_layout_callback = func;
};
void MqttClient::set_values_callback(void (*func)(String) ){
  _cmd_values_callback = func;
};

void MqttClient::inbound_callback(char* topic, uint8_t* payload, unsigned int length){
  // for i in range length -> (char)payload[i]
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]\n");
  String topic_str = String(topic);
  String payload_str = (char*)payload;
  payload_str = payload_str.substring(0, length);

  if(*_cmd_power_callback != NULL && topic_str.equalsIgnoreCase(_power_topic)){
      bool state = payload_str.equalsIgnoreCase("ON");
      log("power command received");
      _cmd_power_callback(state);
  };
  if(*_cmd_bright_callback != NULL && topic_str.equalsIgnoreCase(_bright_topic)){
      int brightness = payload_str.toInt();
      log("bightness command received");
      _cmd_bright_callback(brightness);
  };
  if(*_cmd_layout_callback != NULL && topic_str.equalsIgnoreCase(_layout_topic)){
      log("layout command received");
      _cmd_layout_callback(payload_str);
  };
  if(*_cmd_values_callback != NULL && topic_str.equalsIgnoreCase(_values_topic)){
      log("values command received");
      _cmd_values_callback(payload_str);
  };
};
