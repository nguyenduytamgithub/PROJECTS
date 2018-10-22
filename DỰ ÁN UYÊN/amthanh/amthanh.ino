#include <SD.h>
//#define SD_ChipSelectPin 53  //sử dụng SS Pin 53 trên Mega2560
#define SD_ChipSelectPin 4  //thường sử dụng digital pin 4 trên arduino nano 328, hoặc chân tùy ý
#include <TMRpcm.h>
#include <SPI.h>
TMRpcm tmrpcm;
unsigned long time = 0;
#define bt8 8 // nut nhan chuyen bai
#define NUMBER 7 // so bai hat
int now = 0;
int prev = 0;
int count = 0;
int flash = 0;
void check_sd (){
  if (!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }else{
    Serial.println("OK");
  }
}
void chatluong (){
  tmrpcm.quality(1);
  tmrpcm.volume(1);
}
void active_pin (){
  tmrpcm.speakerPin = 9; //5,6,11 hoặc 46 trên Mega, 9 trên Uno, Nano. Vì sao, xem bài viết cứng của anh NTP_PRO http://arduino.vn/bai-viet/411-timercounter-tren-avrarduino
  pinMode(10,OUTPUT);
}
void setup()
{
  active_pin();
  Serial.begin(115200); //bật serial monitor ở mức baudrate 9600 :)
  check_sd();
  chatluong();
  pinMode(bt8,INPUT_PULLUP);
  tmrpcm.play("wav/xinchao.wav");
  delay(2000);
  time = millis();
}
void check_status (){
  if(millis() - time > 50)
  {
    if (!tmrpcm.isPlaying())
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
    time = millis();
  }
}
void check_stop(){
  if (now == LOW){
     flash ++;
     if (flash > 13000)  // 13k la count hop ly stop
          count = 0;
  }else {
      flash = 0;
  }
}
void loop()
{
  now = digitalRead(bt8);
 
  if (now != prev ){
      if (now == LOW){
        count ++;  
      }
       if (count % NUMBER == 1){
            tmrpcm.play("wav/1.wav"); 
          } else if (count % NUMBER == 2){
            tmrpcm.play("wav/2.wav"); 
          } else if (count % NUMBER == 3){
            tmrpcm.play("wav/3.wav"); 
          } else if (count % NUMBER == 4){
            tmrpcm.play("wav/4.wav"); 
          } else if (count % NUMBER == 5){
            tmrpcm.play("wav/5.wav"); 
          } else if (count % NUMBER == 6){
            tmrpcm.play("wav/6.wav"); 
          }else{
            tmrpcm.stopPlayback();
          }
  }
  prev = now;
  check_stop(); // nut nhan
  check_status(); // stop khi khong hat
}
