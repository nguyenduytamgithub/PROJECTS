extern "C" {
#include "user_interface.h"
}
#include <ESP_EEPROM.h>
int valR;
int a  = 1;
int b = 1;
unsigned long time_count_reset = 0;
int getResetReason() {
  rst_info* ri = system_get_rst_info();
  if (ri == NULL)
    return -1;

  return ri->reason;
}
//=====================   CHECK RESET BUTTON =============
void check_reset (){
  int reasonRst = getResetReason(); 
  EEPROM.get(100, valR);
   if (reasonRst == 6) {
    Serial.println(reasonRst);
    if (valR >= 2) {
      EEPROM.put(100, 0);
     // enter_smartconfig();
      Serial.println("Enter smartconfig");
    }
    else {
      EEPROM.put(100, valR + 1);
      Serial.println("lan nhan 2");
    }

  }
  else {
    Serial.println(reasonRst);
    EEPROM.put(100, 1);
  }
  EEPROM.commit();
}

//===================== CHECK TIME RESET===================
void check_time_reset (){

//  if (millis() - time_count_reset >  5000){
     EEPROM.put(100, 0);
     EEPROM.commit();
     Serial.print("da du reset");
     EEPROM.get(100, valR);
     Serial.println(valR);
     ESP.deepSleep(10e6);
//     time_count_reset = millis();
//  }else{
//    if (b == 1){
//     Serial.print("chua du");
//    Serial.println(valR);
//    b = 0;
//    }
//  }
}
void setup() {
  // put your setup code here, to run once:
  EEPROM.begin(512);
  Serial.begin(115200);
  check_reset ();
  delay(2000);
  check_time_reset();
}
unsigned long timer = 0;
void loop() {
   
//  if(millis()- timer > 10000){
//    Serial.println("du 10s roi di ngu");
//  // put your main code here, to run repeatedly:
//  ESP.deepSleep(10e6);
//  timer = millis();
//  }else {
//    if (a == 1){
//    Serial.println("chua du 10s, tao dang song");
//    Serial.println();
//    a = 0;
//    }
//    }
}
