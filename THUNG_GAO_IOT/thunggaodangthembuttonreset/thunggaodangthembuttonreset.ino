// libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>
#include <ESP_EEPROM.h>

//define
#define id_box 12346 // ma thung
#define TIME_SLEEP 10e4//0xffffffff 
#define TIME_PUBLISH 2000 // khoang cach gui
#define TIME_WAIT 15000 // thoi gian toi da ket noi wifi
#define VCC_ADJ 1.096 //const do pin
#define EEPROM_COUNT_PUSH 444 // so lan da gui
#define EEPROM_COUNT_WAKEUP 333 // so lan ngu day
#define HIGH_MIN 3 // can sua cho nay de duoc 2 kg
#define HET "sắp hết"
#define CON "còn gạo"
#define YEUCAU "yêu cầu"
#define MAGIC_NUMBER 2 //1 //so lan ngu toi da
//#define SERIAL_DEBUG
#define ADC_MIN 0.5
#define ADC_ALERT 2.5
ADC_MODE(ADC_VCC);
extern "C" {
#include "user_interface.h"
}
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
int valR; // bien reset
unsigned long time_publish = 0;
unsigned long timewait = 0;
unsigned long time_count_reset = 0;
// gpio su dung
#define trig D7
#define echo D6
#define power_sensor D5

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
  #ifdef SERIAL_DEBUG
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  #endif
}
//=============== NGU NGAY SAU GUI HOAC OVER TIME ================================
void remember_to_sleep(){
      EEPROM.get(EEPROM_COUNT_WAKEUP, count_wakeup);
      count_wakeup ++;
      if (count_wakeup > MAGIC_NUMBER) count_wakeup = 1;
      EEPROM.put(EEPROM_COUNT_WAKEUP, count_wakeup);
      EEPROM.commit();
      #ifdef SERIAL_DEBUG
      Serial.println("sleeping");
      #endif
      ESP.deepSleep(TIME_SLEEP);
      yield();
}
//================ CALL BACK VA TIEP NHAN LENH NGU TU SERVER ========================
void callback(String topic, byte* payload, unsigned int length) {
  String msg_payload = "";
  for (int i = 0; i < length; i++) {
    #ifdef SERIAL_DEBUG
    Serial.print((char)payload[i]);
    #endif
    msg_payload += ((char)payload[i]);
  }
  //Serial.println();
  if (topic == "ServerToEsp") {
    StaticJsonBuffer<500> jsonBufferReceive;
    JsonObject& jsReceive = jsonBufferReceive.parseObject((char*)payload);
    if (jsReceive["id_box"] == id_box) {
      flag = false;
      if (jsReceive["time_up"] == "ok")
        remember_to_sleep();
      else { //truong hop so ngoai 21 - 3
        int time_up = jsReceive["time_up"];
        count_wakeup = MAGIC_NUMBER - time_up;
        EEPROM.put(EEPROM_COUNT_WAKEUP, count_wakeup);
        EEPROM.commit();
        remember_to_sleep();
        }
    }
  }
}
//=============== VOID KIEM TRA XEM CAC TINH NANG QUA GIO CHUA ===
void check_time_wait(){ // loop, mqtt, read sensor use this function
  if (millis() - timewait > TIME_WAIT){
      #ifdef SERIAL_DEBUG
      Serial.print("Sent fail");
      #endif
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
      #ifdef SERIAL_DEBUG
      Serial.print("connected");
      #endif
      // Once connected, publish an announcement...
      client.subscribe("ServerToEsp");
    } else {
      #ifdef SERIAL_DEBUG
      Serial.print("failed, Reconnect 100ms");
      #endif
      // Wait 100 seconds before retrying
      delay(10);     
    }
   check_time_wait(); // xem qua gio chua
  }
}
//==================== VOID DI NGU NEU CHUA DU MAGIC ========
void check_daily(){
  if (EEPROM.get(EEPROM_COUNT_WAKEUP, count_wakeup) != MAGIC_NUMBER)
    remember_to_sleep();
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
  digitalWrite(power_sensor, 1); // mở chân kích nguồn
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
    check_time_wait(); // kiem tra xem qua gio chua
  }
  digitalWrite(power_sensor, 0); // tắt sung khếch đại
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
      //Serial.begin(115200);
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

// ==================== DOC DATA VA TINH SO KI ======================

float read_data(){
  int distance = get_distance ();
  float soki = caculator(distance);
  eeprom_push(); // NEU GUI DU LIEU LA EEPROM SO LAN PUSH
  return soki;  
}
//=================== VOID GUI DATA ======================
void push_data(float soki){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
    root["id_box"] = id_box;
    root["soki"] = soki;
    root["pin"] = (float)ESP.getVcc()* VCC_ADJ;
    root["trangthai"] = EEPROM.get(EEPROM_COUNT_PUSH, count_push) - ADC_MIN;
    char jsonChar[100];
    root.printTo(jsonChar); 
    client.publish("cntn/iot/esp/log", jsonChar);
}
//=================== VOID ENBABLE WIFI =====
void enable_wifi (){
  WiFi.forceSleepWake();
  WiFi.mode(WIFI_STA);
  yield();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
// ================== VOID DISABLE WIFI =====
void disable_wifi() {
  WiFi.forceSleepBegin();  // send wifi to sleep to reduce power consumption
  yield();
}
//=====================   CHECK RESET BUTTON =============
void check_reset (){
  int reasonRst = getResetReason(); 
  EEPROM.get(100, valR);
   if (reasonRst == 6) {
    if (valR >= 2) {
      EEPROM.put(100, 0);
     // enter_smartconfig();
      Serial.println("Enter smartconfig");
    }
    else {
      EEPROM.put(100, valR + 1);
    }
  }
  else {
    EEPROM.put(100, 1);
  }
  EEPROM.commit();
}
//===================== CHECK TIME RESET===================
void check_time_reset (){
     delay(2000);
     EEPROM.put(100, 0);
     EEPROM.commit();
//     Serial.print("da du reset");
//     EEPROM.get(100, valR);
//     Serial.println(valR);
}
//===================== VOID SETUP ========================
void setup() {
  disable_wifi();
  EEPROM.begin(512);
  check_reset ();
  check_time_reset();
  check_daily();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(power_sensor, OUTPUT);
  float soki = read_data ();
  enable_wifi();
  reconnect();
  push_data(soki);
}
//======================  VOID LOOP =====================
void loop() {
  client.loop();
 check_time_wait(); // xem qua gio chua
}
