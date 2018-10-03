#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>
#define MIN 1.2
#define HIGH_MIN 3
#define id_box 1234
#define HET "sắp hết"
#define CON "còn gạo"
#define YEUCAU   "yêu cầu"
// Update these with values suitable for your network.

const char* ssid = "GE";
const char* password = "1234567891011";
const char* mqtt_server = "192.168.20.127";
int S = 400;
int h = 28;
int trig =  D6;
int echo = D5;
int button = D7;
bool flag = false;
String trangthai = "";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
unsigned long time_push = 0;
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
  String msg_payload = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg_payload += ((char)payload[i]);
  }
  Serial.println();
  if (topic == "ServerToEsp"){   
    StaticJsonBuffer<500> jsonBufferReceive;
    JsonObject& jsReceive = jsonBufferReceive.parseObject((char*)payload);
    if (jsReceive["id_box"] == id_box){
        flag = false;
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
      // Once connected, publish an announcement...
      client.subscribe("ServerToEsp");
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
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(button,INPUT_PULLUP);
}
int get_distance(){
   unsigned int distance;
   unsigned int disSave = 0;
   unsigned long duration; // biến đo thời gian
   int arr[20] = {0};
   int brr[50] = {0};
   int max_arr = 0;
               // biến lưu khoảng cách
    for (int i = 0; i < 20; i++){
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
    if (distance > h || distance < 0)
    disSave = disSave; 
    else
    disSave = distance;
    delay(50);
    arr[i] = disSave;
    /* In kết quả ra Serial Monitor */  
    }
    for (int i = 0; i < 20; i++){
        brr[arr[i]]++;
    }
    for (int i = 0;i<20;i++){
        if(brr[arr[i]]>max_arr) max_arr = brr[arr[i]];  
    }
    for (int i = 0; i<50; i++){
      if (brr[i] == max_arr){ disSave = i;  
      }
    }
    Serial.print(disSave);
    Serial.println("cm");
    return disSave;
}
float caculator (int dis){
  float vol = 0.0;
  float volSave = 0.0;
  int h1 = h-dis;
  vol = (S*h1)/1000.0;
  if (vol < 0)
  volSave = volSave;
  else 
  volSave = vol;
  return volSave;
}
bool checkRice (int vol){
  if (flag == true){
     trangthai = "yêu cầu";
     return false;
  }else{
    if (vol <= MIN) {
      trangthai = "sắp hết";
      return false;
    }else
      trangthai = "còn gạo";
  return true;
 }
}
bool checkRiceHigh (int high){
  if (flag == true){
     trangthai = "yêu cầu";
     return false;
  }else{
    if (h-high <= HIGH_MIN) {
      trangthai = "sắp hết";
      return false;
    }else
      trangthai = "còn gạo";
  return true;
 }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  int distance = get_distance ();
  float soki = caculator(distance);
  Serial.print(soki);
  Serial.println("  g");
  if (digitalRead(button) == 0){
    flag = true;
  }
  if (!checkRiceHigh(distance)){
    //if (!checkRice(soki)){
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["id_box"] = id_box;
    root["trangthai"] = trangthai;
    char jsonChar[100];
    root.printTo(jsonChar);
    client.publish("cntn/iot/esp/thongbao", jsonChar);
  }
   StaticJsonBuffer<200> jsonBuffer;
   JsonObject& root = jsonBuffer.createObject();
   if (millis() - time_push > 3000){
   root["id_box"] = id_box;
   root["soki"] = soki;
   root["trangthai"] = trangthai;
   char jsonChar[100];
   root.printTo(jsonChar);
   time_push = millis();
//    
//   Serial.println(jsonChar);
   client.publish("cntn/iot/esp/log", jsonChar);
   }
}
