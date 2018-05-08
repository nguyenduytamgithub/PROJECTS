
#include <HX711_ADC.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define RANGE_RICE 1000
const char* ssid = "Galaxy K Zoom";
const char* password = "03031997";
const char* mqtt_server = "115.79.27.129";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char ID[50];
char msg[50];
char msg2[50];
unsigned long time1 = 0;


HX711_ADC LoadCell(D1, D2);
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
if (topic == "cntn/iot/esp1/tare"){
  if (msg_data == "1") {
    LoadCell.tareNoDelay();
  }
//  } else {
//   // digitalWrite(D1, 0); 
//    client.publish("ge/esp1/ledstt", "Đang tắt");
//  }
 }else if (topic == "cntn/iot/esp1/calib"){
    Serial.println("Calibing");
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
      //====================================
      client.subscribe("cntn/iot/esp1/tare");
      client.subscribe("cntn/iot/esp1/calib");
      //====================================
      snprintf (ID, 75, "%ld", random(0xffff));
      client.publish("cntn/iot/esp1/id", ID);
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
  Serial.begin(115200);
    
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
         // reset the scale to 0
  LoadCell.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  LoadCell.start(stabilisingtime);
  LoadCell.setCalFactor(91.6); // user set calibration factor (float)
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 

   LoadCell.update();

  //get smoothed value from data set + current calibration factor
if (millis() - time1 > 3000){
    if (LoadCell.getData() < RANGE_RICE) 
        client.publish("cntn/iot/esp1/thongbao", "Đã hết");
     else 
        client.publish("cntn/iot/esp1/thongbao", "Còn đủ sử dụng");
    int gram = LoadCell.getData();
    Serial.print("Load_cell output val: ");
    Serial.println(gram);
    int kg = gram/1000;
    snprintf (msg, 75, "%ld", kg);
    snprintf (msg2, 75, "%ld", kg+1);
    client.publish("cntn/iot/esp1/khoiluong", msg);
    client.publish("cntn/iot/esp1/dudoan", msg2);
    time1 = millis();
}


  //receive from serial terminal
  if (Serial.available() > 0) {
    float i;
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  //check if last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}
