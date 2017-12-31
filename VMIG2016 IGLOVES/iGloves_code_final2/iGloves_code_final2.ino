
#include <SD.h>
//#define SD_ChipSelectPin 53  //sử dụng SS Pin 53 trên Mega2560
#define SD_ChipSelectPin 4  //thường sử dụng digital pin 4 trên arduino nano 328, hoặc chân tùy ý
#include <TMRpcm.h>
#include <SPI.h>
 
TMRpcm tmrpcm;
unsigned long time = 0;
//DEFINE SUM -DEG
#define DEG0 20
#define DEG90 50
#define DEG180 100
#define MAXSUM 500
#define MAXSUM1 450
#define MINSUM  300
#define MINSUM1 200
#define MINSUM2 170
#define MINSUM3 100
//DEFINE MIN
#define MIN1 570
#define MIN2 600
#define MIN3 542
#define MIN4 560
#define MIN5 605
//DEFINE MAX
#define MAX1 760
#define MAX2 795
#define MAX3 795
#define MAX4 800
#define MAX5 812
// DEFINE RANK
#define MIN 0
#define MAX 100
//khai báo
int f1 = A1;
int f2 = A2;
int f3 = A3;
int f4 = A6;
int f5 = A7;
int msg[8];
int av = 0;
int bv = 0;
int cv = 0;
int dv = 0;
int ev = 0;
int va0 = 0;
int va1 = 0;
int va2 = 0;
int va3 = 0;
int va4 = 0;
int va5 = 0;
int vxl = 0;
int vxc = 0;
int vtb = 0;
int vtoi = 0;
int vmen = 0;
int vban = 0;
int vfy = 0;

/*//----------------------------------------------------MPU 6050--------------------------------
 #include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
// MPU control/status vars
bool dmpReady = false; 
uint8_t mpuIntStatus;  
uint8_t devStatus;     
uint16_t packetSize;    
uint16_t fifoCount;     
uint8_t fifoBuffer[64]; 
Quaternion q;          
VectorInt16 aa;        
VectorInt16 aaReal;    
VectorInt16 aaWorld;   
VectorFloat gravity;   
float euler[3];        
float ypr[3];          
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
volatile bool mpuInterrupt = false;  
void dmpDataReady() {
    mpuInterrupt = true;
}
*/
//============================================================VOID SETUP=========================================
void setup()
{

  //----------------------
  pinMode(f1,INPUT);
  pinMode(f2,INPUT);
  pinMode(f3,INPUT);
  pinMode(f4,INPUT);
  pinMode(f5,INPUT);

  //-----------------------

  tmrpcm.speakerPin = 9; //5,6,11 hoặc 46 trên Mega, 9 trên Uno, Nano. Vì sao, xem bài viết cứng của anh NTP_PRO http://arduino.vn/bai-viet/411-timercounter-tren-avrarduino
  pinMode(10,OUTPUT);// Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45 
  
  Serial.begin(115200); //bật serial monitor ở mức baudrate 9600 :)
  if (!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  else
  {
    Serial.println("OK");
  }

 
 /* //----------------------MPU 6050 ------------------------------------------
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24;
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    mpu.initialize();
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
    
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-60);
    mpu.setZAccelOffset(1788); 

    if (devStatus == 0) {
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    }  */
} 
void loop()
{


 //--------------------------
  // construct
  int val1 = analogRead (f1);
  int val2 = analogRead (f2);
  int val3 = analogRead (f3);
  int val4 = analogRead (f4);
  int val5 = analogRead (f5);
  // change value with map
  msg[0] = map(val1,MIN1,MAX1,MIN,MAX);
  msg[1] = map(val2,MIN2,MAX2,MIN,MAX);
  msg[2] = map(val3,MIN3,MAX3,MIN,MAX);
  msg[3] = map(val4,MIN4,MAX4,MIN,MAX);
  msg[4] = map(val5,MIN5,MAX5,MIN,MAX);
     
 //----------------------
 
 /* int sum = msg[0] + msg[1] + msg[2] + msg[3] + msg[4];
  int sum1 = msg[1] + msg[2] + msg[3] + msg[4];
  int sum2 = msg[2] + msg[3] + msg[4];
  int sum3 = msg[3] + msg[4];
     if (sum > MAXSUM){
         tmrpcm.play("1001.wav");
         delay(3500);
        
    }*/
  //-------- A
  if (msg[0] < 20 && msg[1] > 65 && msg[2] > 70 && msg[3] > 70 && msg[4] > 70 ){
    av++;
    if (av > 250){
         tmrpcm.play("/wav/a.wav");
         av = 0;
    }
       
  }
  //-------- B
  if (msg[0] > 30 && msg[0] < 80 && msg[1] < 20 && msg[2] < 20 && msg[3] < 20 && msg[4] < 20 ){
    bv++;
    if (bv > 250){
         tmrpcm.play("/wav/b.wav");
         bv = 0;
    }
       
  }
  //-------- c
  if (msg[0] > 20 && msg[0] < 75 && msg[1] > 30 && msg[1] < 65 && msg[2] > 30 && msg[2] < 75 && msg[3] > 30 && msg[3] < 75 && msg[4] > 50 ){
    cv++;
    if (cv > 250){
         tmrpcm.play("/wav/c.wav");
         cv = 0;
    }
       
  }
  //-------- d
  if (msg[0] > 20 && msg[0] < 65 && msg[1] < 10 && msg[2] > 25 && msg[2] < 65 && msg[3] >25 && msg[3] < 65 && msg[4] > 25 && msg[4] < 70){
    dv++;
    if (dv > 200){
         tmrpcm.play("/wav/d.wav");
         dv = 0;
    }
       
  }
  //-------- e
  if (msg[0] > 75 && msg[1] > 65 && msg[2] > 75 && msg[3] > 75 && msg[4] >75 ){
    ev++;
    if (ev > 250){
         tmrpcm.play("/wav/e.wav");
         ev = 0;
    }
       
  }
   //-------- 1
  if (msg[0] > 70 && msg[1] < 10 && msg[2] > 70 && msg[3] > 70 && msg[4] > 70 ){
    va1++;
    if (va1 > 250){
         tmrpcm.play("/wav/1.wav");
         va1 = 0;
    }
       
  }
  //-------- 2
  if (msg[0] > 30 && msg[1] < 25 && msg[2] < 20 && msg[3] > 60 && msg[4] > 60 ){
    va2++;
    if (va2 > 250){
         tmrpcm.play("/wav/2.wav");
         va2 = 0;
    }
       
  }
  //-------- 3
 if (msg[0] > 30 && msg[1] < 20 && msg[2] < 30 && msg[3] < 30 && msg[4] > 55 ){
    va3++;
    if (va3 > 250){
         tmrpcm.play("/wav/3.wav");
         va3 = 0;
    }
       
  }
  //-------- 4
  if (msg[0] > 80 && msg[1] < 20 && msg[2] < 20 && msg[3] < 20 && msg[4] < 20 ){
    va4++;
    if (va4 > 250){
         tmrpcm.play("/wav/4.wav");
         va4 = 0;
    }
       
  }
  //-------- 5
  if (msg[0] < 10 && msg[1] < 10 && msg[2] < 10 && msg[3] < 10 && msg[4] < 10 ){
    va5++;
    if (va5 > 300){
         tmrpcm.play("/wav/5.wav");
         va5 = 0;
    }
       
  }
  //-------- xlfccn
  if (msg[0] > 50 && msg[1] < 20 && msg[2] > 50 && msg[3] < 20 && msg[4] < 20 ){
    vxl++;
    if (vxl > 250){
         tmrpcm.play("/wav/xlfccn.wav");
         vxl = 0;
    }
       
  }
  //-------- xinchao
  if (msg[0] < 10 && msg[1] < 10 && msg[2] < 10 && msg[3] < 10 && msg[4] > 50 ){
    vxc++;
    if (vxc > 250){
         tmrpcm.play("/wav/xinchao.wav");
         vxc = 0;
    }
       
  }
  //-------- tambiet
  if (msg[0] < 10 && msg[1] < 10 && msg[2] < 10 && msg[3] > 60 && msg[4] < 10 ){
    vtb++;
    if (vtb > 250){
         tmrpcm.play("/wav/tambiet.wav");
         vtb = 0;
    }
       
  }
  //-------- toi
  if (msg[0] < 30 && msg[1] > 55 && msg[2] > 55 && msg[3] > 55 && msg[4] < 10 ){
    vtoi++;
    if (vtoi > 250){
         tmrpcm.play("/wav/toi.wav");
         vtoi = 0;
    }
       
  }
  //-------- ban
  if (msg[0] < 20 && msg[1] < 15 && msg[2] > 65 && msg[3] > 65 && msg[4] > 65 ){
    vban++;
    if (vban > 250){
         tmrpcm.play("/wav/ban.wav");
         vban = 0;
    }
       
  }
  //-------- fuckyou
  if (msg[0] > 50 && msg[1] > 60 && msg[2] < 30 && msg[3] > 60 && msg[4] > 60 ){
    vfy++;
    if (vfy > 250){
         tmrpcm.play("/wav/fuckyou.wav");
         vfy = 0;
    }
       
  }
  //-------- men
  if (msg[0] > 40 && msg[1] > 50 && msg[2] < 20 && msg[3] < 20 && msg[4] < 20 ){
    vmen++;
    if (vmen > 250){
         tmrpcm.play("/wav/men.wav");
         vmen = 0;
    }
       
  }
  
/*//-------------------------------------MPU 6050 --------------------------------------------
  if (!dmpReady) return;
    while (!mpuInterrupt && fifoCount < packetSize) {
    }
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
 if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
              mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
             int m_ypr1 = ypr[1] * 100; 
             int GX = map(m_ypr1,0,130,0,90);
             int m_ypr2 = ypr[2] * 100; 
             int GZ = map(m_ypr2,0,145,0,90);
             int GY = ypr[0] * 180/M_PI;
          //   Serial.println(ypr[0] * 180/M_PI);
           
     msg[5] = GX;
     msg[6] = GY;
     msg[7] = GZ;

 }  
    #endif
  //----------------------*/
   /*Serial.print(val1);
      Serial.print(" "); 
      Serial.print(val2);
      Serial.print(" "); 
      Serial.print(val3);
      Serial.print(" "); 
      Serial.print(val4);
      Serial.print(" "); 
      Serial.print(val5);
      Serial.println(" "); */
     
  printF();
}
void printF(){
      Serial.print(msg[0]);
      Serial.print(" "); 
      Serial.print(msg[1]);
      Serial.print(" "); 
      Serial.print(msg[2]);
      Serial.print(" "); 
      Serial.print(msg[3]);
      Serial.print(" "); 
      Serial.print(msg[4]);
      Serial.print(" "); 
      Serial.print(msg[5]);

   //   Serial.print(" ");
    //  Serial.print(sum);
      Serial.println(" ");  
  
}
