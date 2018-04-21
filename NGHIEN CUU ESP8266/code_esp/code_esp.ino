#include <Wire.h>
#define stick D3
//== button
int stt = 0;
int pre_stt = 0;
int count = 0;
unsigned long time_0 = 0;
unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;
void setup() {
  Wire.begin(D1,D2); // join i2c bus (address optional for master)
  pinMode(stick,INPUT_PULLUP);
  Serial.begin(9600);
}
void count_stick (){
  stt =digitalRead(stick);
  if (stt != pre_stt){
    if (stt == 0){
        count ++;
        Wire.beginTransmission(8); // transmit to device #8
        Wire.write(4);
        Wire.write(count);        // sends five bytes             // sends one byte
        Wire.endTransmission();
      } 
      pre_stt = stt; 
  }
  
  Serial.println(count);
}


void loop() {
  count_stick();
  // 0 target, 1 name, 2 work, 3 id 
//  if (millis() - time_0 > 100){
//    Wire.beginTransmission(8); // transmit to device #8
//    Wire.write(0);
//    Wire.write(100);        // sends five bytes             // sends one byte
//    Wire.endTransmission();    // stop transmitting
//    time_0 = millis();
//  }
//  if (millis() - time_0 > 200){
//    Wire.beginTransmission(8); // transmit to device #8
//    Wire.write(1);
//    Wire.write("NGUYEN DUY TAM");        // sends five bytes             // sends one byte
//    Wire.endTransmission();    // stop transmitting
//    time_1 = millis();
//  }
//  if (millis() - time_0 > 300){
//    Wire.beginTransmission(8); // transmit to device #8
//    Wire.write(2);
//    Wire.write("Boss");        // sends five bytes             // sends one byte
//    Wire.endTransmission();    // stop transmitting
//    time_2 = millis();
//  }
//  if (millis() - time_0 > 400){
//    Wire.beginTransmission(8); // transmit to device #8
//    Wire.write(3);
//    Wire.write("xyz12345");        // sends five bytes             // sends one byte
//    Wire.endTransmission();    // stop transmitting
//    time_3 = millis();
//  }
}
