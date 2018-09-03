#include <ArduinoJson.h>
#include "GEBridge.h"
#define SERIAL_NAME "MOVEMENT"
#include "SerialCommand.h"
GEBridge *globalBridge = GEBridge::sharedInstance;
SerialCommand& sCmd = globalBridge->getSerial();

//====================== CAM BIEN MPU ===================
#include <Wire.h>
#include "compass.h"
long local_right = 0;
long local_left = 0;
float pos_start = 0;
float BIAS_1 = 3.00;
float BIAS_2 = 10.00;
float range_x1 = 0.00;
float range_y1 = 0.00;
float range_x2 = 0.00;
float range_y2 = 0.00;
// ==================== DIEU KHIEN DONG CO ===============
#define MOTOR_LEFT_N1 6
#define MOTOR_LEFT_N2 7
#define MOTOR_RIGHT_N1 8
#define MOTOR_RIGHT_N2 9
#define POWER_MAX 75;
#define POWER_RUN 90;
#define POWER_MEDIUM 55;
#define POWER_CALIB 70;
int POWER_LEFT = POWER_RUN;
int POWER_RIGHT = POWER_RUN;
#define PHASE_LEFT_A 2
#define PHASE_LEFT_B 4
#define PHASE_RIGHT_A 3 //CLK
#define PHASE_RIGHT_B 6 //DATA
#define POWER_LOW 0
#define DEGREE_BARRIER 100
//#define SERIAL_DEBUG_ENCODER
#define SERIAL_DEBUG
int motor_stt = 0; // gom nhieu trang thai
bool flag_motor_degree = true; // bien lay goc ban dau 1 lan
float pos_goal = 0.00;
float degree = 0.00;
int error = 0;
int kp = 21;
unsigned long time_error = 0, time_barrier = 0, time_degree = 0;
bool flag_barrier = true;
//======================= HAM SET POWER CAO =============
void power_max(){
  POWER_LEFT = POWER_MAX;
  POWER_RIGHT = POWER_MAX;
  //Serial.println(75);
}
//======================= HAM SET POWER TRUNG BINH =============
void power_medium(){
  POWER_LEFT = POWER_MEDIUM;
  POWER_RIGHT = POWER_MEDIUM;
  //Serial.println(55);
}
void power_run(){
  POWER_LEFT = POWER_RUN;
  POWER_RIGHT = POWER_RUN;
  //Serial.println(55);
}
void power_calib(){
  POWER_LEFT = POWER_CALIB;
  POWER_RIGHT = POWER_CALIB;
  //Serial.println(65);
}
//======================= HAM TINH VI TRI ENCODER =======
void motor_left()
{
  if (digitalRead(PHASE_LEFT_B) == 1)
    local_left++;
  else
    local_left--;
}
void motor_right()
{
  if (digitalRead(PHASE_RIGHT_B) == 1)
    local_right--;
  else
    local_right++;   
}
//======================== HAM TINH ERROR ENCODER =======
void get_error (){
  error = local_left - local_right;   
}
// ======================= HAM CAI DAT MOTOR ============= 
void setup_motor(){
   attachInterrupt(0, motor_left, RISING);
   attachInterrupt(1, motor_right, RISING);
   pinMode(MOTOR_LEFT_N1,OUTPUT);
   pinMode(MOTOR_LEFT_N2,OUTPUT);
   pinMode(MOTOR_RIGHT_N1,OUTPUT);
   pinMode(MOTOR_RIGHT_N2,OUTPUT);
}
// =========================== SETUP MPU ===================
 void setup_mpu (){
  Wire.begin();
  compass_x_offset = -48.08 ; //-62.43;
  compass_y_offset =  158.01; //-41.14;
  compass_z_offset =  41.86; //-43.97;
  compass_x_gainError = 0.89; //1.12;
  compass_y_gainError = 0.95; //1.13;
  compass_z_gainError = 0.89; //1.03; 
 }
//============================ SETUP COMMAND FUNCTION ======
void setup_command (){
  sCmd.addCommand("LEFT",   turn_left);  // Khi có câu lệnh tên là LED_RED sẽ chạy hàm led_red
  sCmd.addCommand("RIGHT",   turn_right);
  sCmd.addCommand("FORWARD",   move_forward);
  sCmd.addCommand("BACKWARD",   move_backward);
  sCmd.addCommand("STOP",   turn_off);
  sCmd.addCommand("CALIB", calib_mpu);
}
//============================ VOID CALIB MPU ==============
 void calib_mpu(){
  power_calib();
  digitalWrite(MOTOR_LEFT_N1,POWER_LOW);
  digitalWrite(MOTOR_RIGHT_N1,POWER_LOW);
  analogWrite(MOTOR_LEFT_N2,POWER_LEFT);
  analogWrite(MOTOR_RIGHT_N2,POWER_RIGHT);
    
  compass_init(2);
  compass_debug = 1;
  compass_offset_calibration(3);
  turn_off();
 }
//============================ VOID SETUP ==================
void setup() 
{ 
  Serial.begin(115200);
  globalBridge->setup(SERIAL_NAME);
  setup_command ();
  setup_mpu ();
  setup_motor();
}
// ========================= VOID LAY GIA TRI MPU
float read_MPU (){
  compass_scalled_reading();
  compass_heading();
  return bearing;//360.00-bearing;
}
//========================== HAM LAY GIA TRI GOC TU SERIAL ===
float convert_degree (){
  char *arg;
  arg = sCmd.next();
  return atof(arg);  
}
// ========================== HAM LAY GOC SE DEN =============
float get_pos_goal (float pos_start){
  float pos_goal = pos_start + degree;
  if (pos_goal > 360){
      pos_goal-=360;
  }else if(pos_goal < 0){
    pos_goal = 360.00+pos_goal;
  }
  return pos_goal;
}
//========================= HAM KIEM TRA GOC ===
void check_degree (){ 
  float pos_now = read_MPU();
  if (flag_motor_degree == true){
    float gNext = convert_degree();
    if (gNext != 0){
      degree = gNext; // get degree from char arg
      power_max();
    }
    pos_start = read_MPU();
    if (degree < 0)
      degree = -degree;
    if (motor_stt == 3)
      degree = -degree;
    Serial.println(degree);
    pos_goal = get_pos_goal(pos_start);
    range_x1 = pos_goal - BIAS_1;
    range_y1 = pos_goal + BIAS_1;
    range_x2 = pos_goal - BIAS_2;
    range_y2 = pos_goal + BIAS_2;
      Serial.print(range_x1);
      Serial.print("  ");
      Serial.println(range_y1);
      Serial.print(range_x2);
      Serial.print("  ");
      Serial.println(range_y2);
    flag_motor_degree = false;
  } 
    #ifdef SERIAL_DEBUG
      Serial.print(pos_now);
      Serial.print("   ");
      Serial.print(pos_goal);
      Serial.print("   ");
      Serial.println(pos_start);
    #endif
  if (pos_now >= range_x2 && pos_now <= range_y2){
    power_medium();
  }  
  if (pos_now >= range_x1 && pos_now <= range_y1){
    Serial.println("no luon dung");
    flag_motor_degree = true;
    flag_barrier == true;
    turn_off();
    //power_max();
  }
}

void turn_left (){
  motor_stt = 3;
  analogWrite(MOTOR_LEFT_N1,POWER_LEFT);
  analogWrite(MOTOR_RIGHT_N1,POWER_RIGHT);
  digitalWrite(MOTOR_LEFT_N2,POWER_LOW);
  digitalWrite(MOTOR_RIGHT_N2,POWER_LOW);
  //Serial.println("turn_left");
  check_degree ();
}
void turn_right(){
  motor_stt = 4;
  digitalWrite(MOTOR_LEFT_N1,POWER_LOW);
  digitalWrite(MOTOR_RIGHT_N1,POWER_LOW);
  analogWrite(MOTOR_LEFT_N2,POWER_LEFT);
  analogWrite(MOTOR_RIGHT_N2,POWER_RIGHT);
  //Serial.println("turn_right");
  check_degree ();
}
void turn_off (){
  motor_stt = 0;
  power_max();
  digitalWrite(MOTOR_LEFT_N1,POWER_LOW);
  digitalWrite(MOTOR_RIGHT_N1,POWER_LOW);
  digitalWrite(MOTOR_LEFT_N2,POWER_LOW);
  digitalWrite(MOTOR_RIGHT_N2,POWER_LOW);
  //Serial.println("turn_off");
}
void move_forward (){
  motor_stt = 1;
  power_run();
  check_error ();
 // check_degree_error ();
  
  digitalWrite(MOTOR_LEFT_N1,POWER_LOW);
  analogWrite(MOTOR_RIGHT_N1,POWER_RIGHT);
  analogWrite(MOTOR_LEFT_N2,POWER_LEFT);
  digitalWrite(MOTOR_RIGHT_N2,POWER_LOW);
  //Serial.println("forward");
}
void move_backward (){
  motor_stt = 2;
  power_run();
  analogWrite(MOTOR_LEFT_N1,POWER_LEFT);
  digitalWrite(MOTOR_RIGHT_N1,POWER_LOW);
  digitalWrite(MOTOR_LEFT_N2,POWER_LOW);
  analogWrite(MOTOR_RIGHT_N2,POWER_RIGHT);
  //Serial.println("backward");
}
//=================== VOID IMPROVE MOTOR ================
void improve_motor (){
  int mapping_power = error/kp;
 //Serial.println(mapping_power);
  POWER_RIGHT += mapping_power;  
}
//=================== VOID RESET ENCODER ================
void reset_encoder (){
   local_right = 0;
   local_left = 0;
}
//=================== VOID KIEM TRA ERROR ===============
void check_error (){
  if (millis() - time_error > 5000){
     get_error();
     improve_motor();
     reset_encoder ();
   time_error = millis();
   }  
}
// ================== VOID KIEM TRA DO LECH =============
void check_degree_error (){
  if (millis() - time_degree > 2000){
      if (flag_barrier == true){
        pos_start = read_MPU();
        flag_barrier = false;  
      }
        float pos_now = read_MPU();
        degree = pos_start - pos_now;
      Serial.print(pos_now);
      Serial.print("   ");
      Serial.print(degree);
      Serial.print("   ");
      Serial.println(pos_start);
      if (degree >= BIAS_2 || degree <= -BIAS_2){
        Serial.println("chuyen goc");
          turn_off();
          if (degree > 0){
            turn_right();
          }else{
            turn_left();
          }
          move_forward();
      }
      
      time_degree = millis();
  }  
      
}
//=================== VOID LOOP =========================
void loop() 
{
  globalBridge->loop();
   //move_forward();
   #ifdef SERIAL_DEBUG_ENCODER
   Serial.print(local_left);
   Serial.print(" ");
   Serial.print(local_right);
   Serial.print(" ");
   Serial.print(error);
   Serial.print(" ");
   Serial.println(POWER_RIGHT);
   #endif
  switch (motor_stt){
    case 1:
    move_forward();
    break;  
    case 2:
    move_backward();
    break;  
    case 3:
    turn_left();
    break;  
    case 4:
    turn_right();
    break;  
   // default:
   //turn_off();
  }
}
