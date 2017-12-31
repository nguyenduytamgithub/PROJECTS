#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>
TMRpcm tmrpcm;   // create an object for use in this sketch
//=======================
#define BT1 2;
#define BT2 3;
#define BT2 4;
struct in_Bt {
  int stt_Bt;
  int next_Bt;
  int prev_Bt; 
};
void setup(){
  Serial.begin(9600);
  tmrpcm.speakerPin = 6; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  pinMode (7,OUTPUT);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }else{
    Serial.println("OK");
  }
  tmrpcm.play("Welcome"); //the sound file "music" will play each time the arduino powers up, or is reset

  pinMode(BT1,INPUT_PULLUP);
  pinMode(BT2,INPUT_PULLUP);
  pinMode(BT3,INPUT_PULLUP);
}

void loop(){  
  int stt_bt1 = digitalRead(BT1);
  int stt_bt2 = digitalRead(BT2);
  int stt_bt3 = digitalRead(BT3);

  

  

}
