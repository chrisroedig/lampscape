#include <MqttClient.h>

MqttClient::MqttClient(){
  _wifi_client = WiFiClient();
  _client = PubSubClient(_wifi_client);

};

MqttClient::~MqttClient(){};

bool MqttClient::setup(char * mqtt_server, char *  device_name){
  using namespace std::placeholders;
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("no wifi connections");
    return false;
  }
  _device_name = device_name;
  Serial.printf("wifi connected: setting up mqtt on %s \n", mqtt_server);
  _client.setServer("rileypi.local", 1883);
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
    // Create a random client ID
    String clientId = "lampscape-";
    clientId += String(random(0xffff), HEX);
    Serial.printf("MQTT connecting as %s \n ", clientId.c_str());

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


void MqttClient::log(String message){
  String msg = "["+String(_device_name)+"]: "+String(message);
  String topic = "lampscape/log/"+String(_device_name);
  publish(topic.c_str(), msg.c_str());
}
void MqttClient::announce_boot(){
  publish("lampscape/boot/" + String(_device_name), _device_name);
  Serial.println("MQTT announced boot");
}
void MqttClient::publish_state(String topic, String message){
  String full_topic = "state/lampscape/"+String(_device_name);
  _client.publish(full_topic.c_str(), message.c_str());
};
void MqttClient::publish(String topic, String message){
  Serial.println("MQTT Publish "+ topic);
  _client.publish(topic.c_str(), message.c_str());
};

void MqttClient::subscribe(){
  _power_topic = "cmnd/"+String(_device_name)+"/POWER";
  _bright_topic = "cmnd/"+String(_device_name)+"/BRIGHT";
  _mode_topic = "cmnd/"+String(_device_name)+"/MODE";
  _palette_topic = "cmnd/"+String(_device_name)+"/PALETTE";
  _params_topic = "cmnd/"+String(_device_name)+"/PARAMS";
  _client.subscribe(_power_topic.c_str());
  _client.subscribe(_bright_topic.c_str());
  _client.subscribe(_mode_topic.c_str());
  _client.subscribe(_palette_topic.c_str());
  _client.subscribe(_params_topic.c_str());
  log("subscribed");
  Serial.println("MQTT subscribed");
  announce_boot();
}


void MqttClient::set_power_callback(void (*func)(bool) ){
  _cmd_power_callback = func;
};
void MqttClient::set_bright_callback(void (*func)(int) ){
  _cmd_bright_callback = func;
};
void MqttClient::set_mode_callback(void (*func)(int) ){
  _cmd_mode_callback = func;
};
void MqttClient::set_palette_callback(void (*func)(int32[4]) ){
  _cmd_palette_callback = func;
};
void MqttClient::set_params_callback(void (*func)(int[8]) ){
  _cmd_params_callback = func;
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
  if(*_cmd_mode_callback != NULL && topic_str.equalsIgnoreCase(_mode_topic)){
      int mode = payload_str.toInt();
      log("mode command received");
      _cmd_mode_callback(mode);
  };
  if(*_cmd_palette_callback != NULL && topic_str.equalsIgnoreCase(_palette_topic)){
      log("palette command received");
      
      StaticJsonDocument<64> doc;
      deserializeJson(doc, payload_str);

      int32 colors[4] = {doc[0], doc[1], doc[2], doc[3]};
      _cmd_palette_callback(colors);
  };
  if(*_cmd_params_callback != NULL && topic_str.equalsIgnoreCase(_params_topic)){
      log("params command received");
      StaticJsonDocument<128> doc;
      deserializeJson(doc, payload_str);
      int params[8] = {
        doc[0], doc[1], doc[2], doc[3],
        doc[4], doc[5], doc[6], doc[7]
        };
      log("params command received");
      
      _cmd_params_callback(params);
  };
};
