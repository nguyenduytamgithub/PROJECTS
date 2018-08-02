// libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>
#include <ESP_EEPROM.h>
//define
#define id_box 12345 // ma thung
#define TIME_SLEEP 0xffffffff 
#define TIME_PUBLISH 2000 // khoang cach gui
#define TIME_WAIT 15000 // thoi gian toi da ket noi wifi
#define VCC_ADJ 1.096 //const do pin
#define EEPROM_COUNT_PUSH 222 // so lan da gui
#define EEPROM_COUNT_WAKEUP 111 // so lan ngu day
#define HIGH_MIN 3 // can sua cho nay de duoc 2 kg
#define HET "sắp hết"
#define CON "còn gạo"
#define YEUCAU "yêu cầu"
#define MAGIC_NUMBER 21 //so lan ngu toi da

ADC_MODE(ADC_VCC);
WiFiClient espClient;
PubSubClient client(espClient);
// wifi va server
const char* ssid = "GE";
const char* password = "1234567891011";
const char* mqtt_server = "192.168.20.127";
// bien toan cuc
int S = 400;
int h = 28;
bool flag = false;
String trangthai = "";
int count_wakeup;  
int count_push;
long lastMsg = 0;
char msg[50];
unsigned long time_publish = 0;
unsigned long timewait = 0;
// gpio su dung
#define trig D7
#define echo D6
#define power_sensor D5
//============== HAM CAI DAT WIFI =============
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  #ifdef SERIAL_DEBUG
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  #ifdef SERIAL_DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  #endif
}
//================ CALL BACK VA TIEP NHAN LENH NGU TU SERVER ========================
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
  if (topic == "ServerToEsp") {
    StaticJsonBuffer<500> jsonBufferReceive;
    JsonObject& jsReceive = jsonBufferReceive.parseObject((char*)payload);
    if (jsReceive["id_box"] == id_box) {
      flag = false;
      if (jsReceive["time_up"] == "ok")
        sleep_then_push();
      else { //truong hop so ngoai 21 - 3
        int time_up = jsReceive["time_up"];
        count_wakeup = MAGIC_NUMBER - time_up;
        EEPROM.put(EEPROM_COUNT_WAKEUP, count_wakeup);
        EEPROM.commit();
        sleep_then_push();
        }
    }
  }
}
//================ NGU KHI KHONG KET NOI HOAC KHONG GUI DUOC =========
void remember_to_sleep(){
  if (millis() - timewait > TIME_WAIT){
      EEPROM.get(EEPROM_COUNT_WAKEUP, count_wakeup);
      count_wakeup ++;
      if (count_wakeup > MAGIC_NUMBER) count_wakeup = 1;
      EEPROM.put(EEPROM_COUNT_WAKEUP, count_wakeup);
      EEPROM.commit();
      timewait = millis();
      Serial.println("sent fail sleep");
      //ESP.deepSleep(0xffffffff);
      ESP.deepSleep(TIME_SLEEP);
      
    }
}
//=============== NGU NGAY SAU GUI ================================
void sleep_then_push(){
      EEPROM.get(EEPROM_COUNT_WAKEUP, count_wakeup);
      count_wakeup ++;
      if (count_wakeup > MAGIC_NUMBER) count_wakeup = 1;
      EEPROM.put(EEPROM_COUNT_WAKEUP, count_wakeup);
      EEPROM.commit();
      Serial.println("sent ok sleep");
      //ESP.deepSleep(0xffffffff);
      ESP.deepSleep(TIME_SLEEP);
}
//=============== RECONNCET VA NGU NEU QUA THOI GIAN =======
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
      #ifdef SERIAL_DEBUG
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 100 milliseconds");
      #endif
      // Wait 100 seconds before retrying
      delay(100);
      
    }
    remember_to_sleep();
  }
}
//==================== VOID DI NGU NEU CHUA DU MAGIC ========
void check_daily(){
  if (EEPROM.get(EEPROM_COUNT_WAKEUP, count_wakeup) != MAGIC_NUMBER)
    sleep_then_push();
}
//======================== VOID EEPROM SO LAN PUSH =========
void eeprom_push(){
    EEPROM.get(EEPROM_COUNT_PUSH, count_push);
    count_push++;
    EEPROM.put(EEPROM_COUNT_PUSH, count_push);
    EEPROM.commit();
}
//============== INT TINH KHOANG CACH VA LOC NHIEU ============
int get_distance() {
  unsigned int distance;
  unsigned int disSave = 0;
  unsigned long duration; // biến đo thời gian
  int arr[20] = {0};
  int brr[50] = {0};
  int max_arr = 0;
  // biến lưu khoảng cách
  for (int i = 0; i < 20; i++) {
    /* Phát xung từ chân trig */
    digitalWrite(trig, 0);  // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig, 1);  // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig, 0);  // tắt chân trig

    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo.
    duration = pulseIn(echo, HIGH);
    // Tính khoảng cách đến vật.
    distance = int(duration / 2 / 29.412);
    if (distance > h || distance < 0)
      disSave = disSave;
    else
      disSave = distance;
    delay(10);
    arr[i] = disSave;
    /* In kết quả ra Serial Monitor */
  }
  for (int i = 0; i < 20; i++) {
    brr[arr[i]]++;
  }
  for (int i = 0; i < 20; i++) {
    if (brr[arr[i]] > max_arr) max_arr = brr[arr[i]];
  }
  for (int i = 0; i < 50; i++) {
    if (brr[i] == max_arr) {
      disSave = i;
    }
  }
  #ifdef SERIAL_DEBUG
      Serial.print(disSave);
      Serial.println("cm");
  #endif
  return disSave;
}
//================ VOID TINH THE TICH ================
float caculator (int dis) {
  float vol = 0.0;
  float volSave = 0.0;
  int h1 = h - dis;
  vol = (S * h1) / 1000.0;
  if (vol < 0)
    volSave = volSave;
  else
    volSave = vol;
  return volSave;
}
//=============== VOID KIEM TRA GAO THEO CHIEU CAO
bool checkRiceHigh (int high) {
  if (flag == true) {
    trangthai = "yêu cầu";
    return false;
  } else {
    if (h - high <= HIGH_MIN) {
      trangthai = "sắp hết";
      return false;
    } else
      trangthai = "còn gạo";
    return true;
  }
}
//=================== VOID GUI DATA ======================
void push_data(){
  if (millis() - time_publish > TIME_PUBLISH){
  int distance = get_distance ();
  float soki = caculator(distance);
  eeprom_push(); // NEU GUI DU LIEU LA EEPROM SO LAN PUSH
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
    root["id_box"] = id_box;
    root["soki"] = soki;
    root["pin"] = (float)ESP.getVcc()* VCC_ADJ;
    root["trangthai"] = EEPROM.get(EEPROM_COUNT_PUSH, count_push);
    char jsonChar[100];
    root.printTo(jsonChar);
    client.publish("cntn/iot/esp/log", jsonChar);
  time_publish = millis();
  }
}
// ==================== KET NOI WIFI ======================

//===================== VOID SETUP ========================
void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  check_daily();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(power_sensor, OUTPUT);
}
//======================  VOID LOOP =====================
void loop() {
//  if (!client.connected()) {
//    reconnect();
//  }
//  client.loop();
  push_data();
  remember_to_sleep(); // van ngu neu nhu khong phan hoi server

}
