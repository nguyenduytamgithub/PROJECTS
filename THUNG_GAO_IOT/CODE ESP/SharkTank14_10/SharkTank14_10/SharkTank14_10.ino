// libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>

//define
#define id_box 1234
char* rBox_topic = "rBox/" ;

#define MIN_TIME 5e6//0xffffffff 
#define MAX_TIME 0xffffffff
unsigned TIME_SLEEP = MIN_TIME;
#define TIME_WAIT 15000 // thoi gian toi da ket noi wifi
#define VCC_ADJ 1.096 //const do pin
ADC_MODE(ADC_VCC);
extern "C" {
#include "user_interface.h"
}
WiFiClient espClient;
PubSubClient client(espClient);
// wifi va server
const char* ssid = "NDT";
const char* password = "galaxyace5830";
const char* mqtt_server = "14.237.32.42";
char msg[50];
unsigned long timewait = 0;
unsigned long led_start_on = 0;
unsigned long led_start_off = 0;
int getResetReason() {
  rst_info* ri = system_get_rst_info();
  if (ri == NULL)
    return -1;
  return ri->reason;
}
//============== HAM CAI DAT WIFI =============
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  randomSeed(micros());
 // #ifdef SERIAL_DEBUG
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    check_time_wait();
  }
  // #ifdef SERIAL_DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 // #endif
}
//=============== NGU NGAY SAU GUI HOAC OVER TIME ================================
void remember_to_sleep(){
 
    //  #ifdef SERIAL_DEBUG
      Serial.println("sleeping");
    //  #endif
      ESP.deepSleep(TIME_SLEEP);
      yield();
}
//================ CALL BACK VA TIEP NHAN LENH NGU TU SERVER ========================
void callback(String topic, byte* payload, unsigned int length) {
  String msg_payload = "";
  for (int i = 0; i < length; i++) {
   // #ifdef SERIAL_DEBUG
    Serial.print((char)payload[i]);
   // #endif
    msg_payload += ((char)payload[i]);
  }
  //Serial.println();
  if (topic == rBox_topic) {
    StaticJsonBuffer<500> jsonBufferReceive;
    JsonObject& jsReceive = jsonBufferReceive.parseObject((char*)payload);
   //if (jsReceive["id_box"] == id_box) 
        remember_to_sleep();
  }
}
//=============== VOID KIEM TRA XEM CAC TINH NANG QUA GIO CHUA ===
void check_time_wait(){ // loop, mqtt, read sensor use this function
  if (millis() - timewait > TIME_WAIT){
   //   #ifdef SERIAL_DEBUG
      Serial.print("Sent fail");
   //   #endif
      remember_to_sleep(); 
      timewait = millis();
   }
}
//=============== RECONNCET VA NGU NEU QUA THOI GIAN =======
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
  //    #ifdef SERIAL_DEBUG
      Serial.print("connected");
  //    #endif
      // Once connected, publish an announcement...   
      client.subscribe(rBox_topic);
    } else {
  //    #ifdef SERIAL_DEBUG
      Serial.print("failed, Reconnect 100ms");
  //    #endif
      // Wait 100 seconds before retrying
      delay(10);     
    }
   check_time_wait(); // xem qua gio chua
  }
}
//=================== VOID GUI DATA ======================
void push_data(){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
    root["id_box"] = id_box;
    root["pin"] = (float)ESP.getVcc()* VCC_ADJ;
    char jsonChar[100];
    root.printTo(jsonChar); 
    client.publish("rBox/log", jsonChar);
}
void connect_wifi(){
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
//=================== VOID ENBABLE WIFI =====
void enable_wifi (){
  WiFi.forceSleepWake();
  WiFi.mode(WIFI_STA);
  yield();
}
// ================== VOID DISABLE WIFI =====
void disable_wifi() {
  WiFi.forceSleepBegin();  // send wifi to sleep to reduce power consumption
  yield();
}
void led_start(){
  pinMode(2,OUTPUT);
  digitalWrite(2,0);
  delay(100);
  digitalWrite(2,1);
  delay(100);
  digitalWrite(2,0);
  delay(100);
  digitalWrite(2,1);
  delay(100);
  digitalWrite(2,0);
  delay(100);
  digitalWrite(2,1);
  delay(100);
  digitalWrite(2,0);
  delay(100);
  digitalWrite(2,1);
  delay(100);
  digitalWrite(2,0);
  delay(100);
  digitalWrite(2,1);
}
//=====================   CHECK RESET BUTTON =============
void check_reset (){
  int reasonRst = getResetReason(); 
  Serial.println(reasonRst);
   if (reasonRst == 6) {
      Serial.println("Enter smartconfig");
    }
 }
//===================== CHECK TIME RESET===================
void check_time_reset (){
     delay(1000);
}
//===================== MERGE RBOX TOPIC =================
void merger_rBox_topic (){
  snprintf (msg, 75, "%ld", id_box);
  strcat(rBox_topic,msg);  
}
//===================== VOID SETUP ========================
void setup() {
  disable_wifi();
  Serial.begin(115200);
  merger_rBox_topic();
  led_start();
  enable_wifi();
  check_reset();
  connect_wifi();
  //check_time_reset();
  reconnect();
  push_data();
}
//======================  VOID LOOP =====================
void loop() {
  client.loop();
 check_time_wait(); // xem qua gio chua
}
