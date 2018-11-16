#include<ArduinoJson.h> // thu vien giao tiep
int val_nhietdo = 35;
float val_doam = 29.05;
unsigned long timer = 0; // bien thoi gian de gui
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - timer > 3000){
    timer = millis ();
    StaticJsonBuffer<200> jsonBuffer; // khai bao
    JsonObject& root = jsonBuffer.createObject(); // khai bao
    root["NHIETDO"] = val_nhietdo; // them nhiet do
    root["DOAM"] = val_doam; // them do am
    char jsonChar[100];  // khai bao chuoi 100 ki tu
    root.printTo(jsonChar); // tao 1 chuoi hoan chinh
    Serial.println(jsonChar); // in ra
  } 
}
