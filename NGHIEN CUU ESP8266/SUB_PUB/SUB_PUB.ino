
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

const char* ssid = "simone";
const char* password = "1234567891011";
const char* mqtt_server = "192.168.0.100";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void sent_msg (String msg_data, int code){
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(code);
    Wire.write(msg_data.c_str());        // sends five bytes             // sends one byte
    Wire.endTransmission();   
}
void callback(String topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg_data;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg_data += (char)payload[i];
  }
  Serial.println();
if (topic == "counting/name"){
    sent_msg(msg_data,11);
 }else if (topic == "counting/id"){
    sent_msg(msg_data,12);
 }else if (topic == "counting/work"){
   sent_msg(msg_data,13);
 }else if (topic == "counting/target"){
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(14);
    Wire.write(msg_data.toInt());        // sends five bytes             // sends one byte
    Wire.endTransmission(); 
 }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("counting/name");
      client.subscribe("counting/id");
      client.subscribe("counting/work");
      client.subscribe("counting/target");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  Wire.begin(D1,D2);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
//
  long now = millis();
    if (now - lastMsg > 1000) {
    lastMsg = now;
    value = digitalRead(D1);
    //int val_map = map(value,0,1024,0,100);
    snprintf (msg, 75, "%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg); // gui du lieu len server
  }
}
