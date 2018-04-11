
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "GE";
const char* password = "1234567891011";
const char* mqtt_server = "192.168.20.130";

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
if (topic == "ge/esp1/led"){
   Serial.println(msg_data);
  if (msg_data == "1") {
    digitalWrite(D1, 1);   // Turn the LED on (Note that LOW is the voltage level
    client.publish("ge/esp1/ledstt", "Đang bật");

  } else {
    digitalWrite(D1, 0); 
    client.publish("ge/esp1/ledstt", "Đang tắt");
  }
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
      client.subscribe("ge/esp1/led");
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
  pinMode(D1, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
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
//  long now = millis();
//    if (now - lastMsg > 0) {
//    lastMsg = now;
//    value = analogRead(A0);
//    int val_map = map(value,0,1024,0,100);
//    snprintf (msg, 75, "%ld", val_map);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    client.publish("outTopic", msg); // gui du lieu len server
//  }
}
