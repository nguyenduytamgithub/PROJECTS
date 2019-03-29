//============================================================= THU VIEN ================================
#include <ESP8266WiFi.h>
//============================================================= DEFINE ==================================
#define transistor D5 // chan kich transistor
#define echo D6 // sieu am
#define trig D7 // sieu am
#define h_box 28 // chieu cao thung
#define VCC_ADJ 1.096 // he so pin ADC
//============================================================= KHAI BAO BIEN ==========================
String MAC = WiFi.macAddress();
ADC_MODE(ADC_VCC);
float battery = (float)ESP.getVcc()* VCC_ADJ;
//============================================================= VOID SETUP ==============================
void setup (){
    Serial.begin(115200);
    pinMode(transistor,OUTPUT);
    pinMode(echo,INPUT);
    pinMode(trig,OUTPUT);
    pinMode(transistor,OUTPUT); // chan kich sieu am hoat dong 3.3v
    digitalWrite(transistor,1); // kich hoat transistor
}
//============== INT TINH KHOANG CACH VA LOC NHIEU ====================================================
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
    if (distance > h_box || distance < 0)
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
      Serial.print(disSave);
      Serial.println("cm");
  return disSave;
}
// =========================================================== VOID LOOP ==============================
void loop (){
  Serial.println();
}
