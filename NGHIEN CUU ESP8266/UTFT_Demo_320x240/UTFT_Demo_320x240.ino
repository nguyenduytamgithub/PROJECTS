
#include <Wire.h>
#include <UTFT.h>
#define led_work 9
int stt_led = 0;
unsigned long check_time = 0;
String ID = "ID: ";
String Work,Name;
int Target = 10;
int Total = 0;
float Cycle = 0;
int arr[11] = {0};
int time_slot[11];
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];


UTFT myGLCD(ILI9341_16,38,39,40,41);

void receiveEvent(int howMany) {
  int first = Wire.read();
  if (first == 3){ 
    Serial.println("ok");
    ID = "ID: ";
    while (Wire.available()){ // loop through all but the last
      char c = Wire.read();
      ID += c;
    }
      Serial.println(ID);
  }else if (first == 2){ 
    Work = "Work: ";
    while (Wire.available()){ // loop through all but the last
      char c = Wire.read();
      Work += c; 
    }
    Serial.println(Work);
  }else if(first == 1){ 
    Name = "Name: ";
    while (Wire.available()){ // loop through all but the last
      char c = Wire.read();
      Name += c;
    }
    Serial.println(Name);
  } else if (first == 0){ 
    Target = Wire.read();
    Serial.println(Target);
  }
   else if (first == 4){ 
    myGLCD.clrScr();
    Total = Wire.read();
    Serial.println(Total);
  }
//  for (int j = 0; j < howMany; j++){
//    Serial.print(id[j]);  
//  }
Serial.print("  ");
Serial.print(howMany);
Serial.println();
}
void setup()
{
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Name = "Name: ";
  Work = "Work: ";
  ID = "ID: ";
  randomSeed(analogRead(0));
  pinMode(led_work,9);
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.setColor(0, 0, 0); // nền chữ đen
  myGLCD.fillRect(1, 15, 318, 224); // tọa độ x, y, độ dài, độ rộng
}
void caculator (){
  //for (int i = 0; i < 10; i++)
  Cycle = (float)60/Target;  
}
//void count_stick (int &count){
//  if (stt != pre_stt){
//    if (stt == 0){
//        count ++;
//        Total++;
//        if (millis()- check_time > (int)Cycle*1000){
//          if (stt_led == 0){  
//            stt_led = 1;
//            }
//            Serial.println(millis() - check_time);
//          check_time = millis();  
//        }else{
//          stt_led = 0; 
//            
//          }
//        Serial.print("  ");
//        Serial.println(stt_led);
//        
//        
//        digitalWrite(led_work,stt_led);
//        myGLCD.clrScr();
//      } 
//      pre_stt = stt; 
//  }
//}

void loop()
{
  myGLCD.setFont(SmallFont);
  caculator();
  int buf[318];
  int x, x2;
  int y, y2;
  int r;
//====
  
//  count_stick(arr[8]);
//  Serial.print(stt);
//  Serial.print("  ");
//  Serial.println(Total);
// Clear the screen and draw the frame
  //myGLCD.clrScr();
  
  myGLCD.setColor(255, 0, 0); // màu nền tổng thể
  myGLCD.fillRect(0, 0, 319, 14); // tọa độ x, y, độ dài, độ rộng
  myGLCD.setColor(62, 218, 15); // màu nền tổng thể xanh
  myGLCD.fillRect(0, 226, 319, 239); //phạm vi tô màu xanh
  myGLCD.setColor(255, 255, 255); // màu chữ trắng
  myGLCD.setBackColor(255, 0, 0); // nền chữ tím
  myGLCD.print("* Personal Counting Card *", CENTER, 1); //tọa độ in chữ
  myGLCD.setBackColor(62, 218, 15); // nền xanh
  myGLCD.setColor(255,255,0); // chữ vàng
  myGLCD.print("http://www.goldeneyetech.com.vn", CENTER, 227);


  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(0, 14, 319, 226); //vẽ khung
  
  
//119,159
// Draw crosshairs
  myGLCD.setColor(255, 255, 0); // xanh nét 
  myGLCD.setBackColor(0, 0, 0); // background trắng
  myGLCD.print(Name, 5 , 22); //tọa độ in chữ
  myGLCD.print(ID, 217 , 22); //tọa độ in chữ
  myGLCD.print(Work, 5 , 48); //tọa độ in chữ
  myGLCD.print("Target", 217 , 48); //tọa độ in chữ
  myGLCD.setColor(255, 255, 255); // xanh nét 
  myGLCD.print("7:30", 10 , 79); //tọa độ in chữ
  myGLCD.print("8:30", 10 , 100); //tọa độ in chữ
  myGLCD.print("13:30", 10 , 157); //tọa độ in chữ
  myGLCD.print("14:30", 10 , 178); //tọa độ in chữ
  //========= cot thu 2
  myGLCD.print("8:30", 63 , 79); //tọa độ in chữ
  myGLCD.print("9:30", 63 , 100); //tọa độ in chữ
  myGLCD.print("14:30", 63 , 157); //tọa độ in chữ
  myGLCD.print("15:30", 63 , 178); //tọa độ in chữ
  
  //========= cot thu 3
  myGLCD.print("9:30", 116 , 79); //tọa độ in chữ
  myGLCD.print("10:30", 116 , 100); //tọa độ in chữ
  myGLCD.print("15:30", 116 , 157); //tọa độ in chữ
  myGLCD.print("16:30", 116 , 178); //tọa độ in chữ
  //========= cot thu 4
  myGLCD.print("10:30", 169 , 79); //tọa độ in chữ
  myGLCD.print("11:30", 169 , 100); //tọa độ in chữ
  myGLCD.print("17:30", 169 , 157); //tọa độ in chữ
  myGLCD.print("18:30", 169 , 178); //tọa độ in chữ
  //========= cot thu 5
  myGLCD.print("12:30", 222 , 79); //tọa độ in chữ
  myGLCD.print("13:30", 222 , 100); //tọa độ in chữ
  myGLCD.print("18:30", 222 , 157); //tọa độ in chữ
  myGLCD.print("19:30", 222 , 178); //tọa độ in chữ
    
  //========
  myGLCD.setColor(255, 255, 0); // xanh nét 
  myGLCD.print("Total", 275 , 89); //tọa độ in chữ
  myGLCD.print("Cycle", 275 , 166); //tọa độ in chữ
  //========
  //doc
  myGLCD.setColor(255, 255, 255); // xanh nét
  myGLCD.drawLine(53, 67, 53, 224);
  myGLCD.drawLine(106, 67, 106, 224);
  myGLCD.drawLine(159, 67, 159, 224);
  myGLCD.drawLine(212, 15, 212, 224); // thanh doc
  myGLCD.drawLine(265, 41, 265, 224);
  // ngang
  myGLCD.drawLine(1, 41, 318, 41);
  myGLCD.drawLine(1, 67, 318, 67);
  myGLCD.drawLine(1, 119, 318, 119);
  myGLCD.drawLine(1, 145, 318, 145);
  myGLCD.drawLine(1, 197, 318, 197);
  //========
  myGLCD.setFont(BigFont);
  //========= cot thu 6
  myGLCD.setColor(0, 200, 255); // xanh nét
  myGLCD.print(String(Total), 268 , 126); //tọa độ in chữ
  myGLCD.print(String(Cycle), 268 , 205); //tọa độ in chữ
  myGLCD.print(String(Target), 268 , 48); //tọa độ in chữ
  //=======================
  myGLCD.setColor(0, 255, 60); // xanh nét
  myGLCD.print(String(arr[1]), 3 , 126); //tọa độ in chữ -20
  myGLCD.print(String(arr[6]), 3 , 205); //tọa độ in chữ - 20
  myGLCD.print(String(arr[2]), 57 , 126); //tọa độ in chữ -73
  myGLCD.print(String(arr[7]), 57 , 205); //tọa độ in chữ -73
  myGLCD.print(String(arr[3]), 109 , 126); //tọa độ in chữ -126
  myGLCD.print(String(arr[8]), 109 , 205); //tọa độ in chữ -126
  myGLCD.print(String(arr[4]), 162 , 205); //tọa độ in chữ -179
  myGLCD.print(String(arr[5]), 215 , 205); //tọa độ in chữ - 232
  myGLCD.print(String(arr[9]), 162 , 126); //tọa độ in chữ - 179
  myGLCD.print(String(arr[10]), 215 , 126); //tọa độ in chữ -232
//  while(1){}
}
