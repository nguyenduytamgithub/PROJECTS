// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "GE";
const char* password = "1234567891011";
// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.20.137";
// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);
// Connect an LED to each GPIO of your ESP8266
const int ledGPIO2 = D1;
const int ledGPIO5 = D4;
// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
delay(10);
// We start by connecting to a WiFi network
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.print("WiFi connected - ESP IP address: ");
Serial.println(WiFi.localIP());
}
// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
Serial.print(topic);
Serial.print(". Message: ");
String messageTemp;
for (int i = 0; i < length; i++) {
Serial.print((char)message[i]);
messageTemp += (char)message[i];
}
Serial.println();
// Feel free to add more if statements to control more GPIOs with MQTT
// If a message is received on the topic home/office/esp1/gpio2, you check if the message is either 1 or 0. Turns the
//ESP GPIO according to the message
if(topic=="office/d3"){
  if(messageTemp == "1"){
Serial.print("Changing GPIO 2 to ");
digitalWrite(ledGPIO2,1);
Serial.print(messageTemp);
}else{

digitalWrite(ledGPIO2,0);
Serial.print(messageTemp);
     }
  }

if(topic=="office/d4"){
Serial.print("Changing GPIO 5 to ");
analogWrite(ledGPIO5, messageTemp.toInt()); //chuyen sang int
Serial.print("On");
}
Serial.println();
}
// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect() {
  // Loop until we're reconnected
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
// Attempt to connect
if (client.connect("ESP8266Client")) {
Serial.println("connected");
// Subscribe or resubscribe to a topic
// You can subscribe to more topics (to control more LEDs in this example)
client.subscribe("office/d3");
client.subscribe("office/d4");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
// Wait 5 seconds before retrying
delay(5000);
}
}
}
// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
pinMode(ledGPIO2, OUTPUT);
pinMode(ledGPIO5, OUTPUT);
Serial.begin(115200);
setup_wifi();
client.setServer(mqtt_server, 1883);
client.setCallback(callback);
}
// For this project, you don't need to change anything in the loop function.
// Basically it ensures that you ESP is connected to your broker
void loop() {
if (!client.connected()) {
reconnect();
}
client.loop();
}

