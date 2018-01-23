int sensor = A0; // chan su dung
int count = 0; // so vong quay
int mutex = 1; // chan count nhieu lan
int mutex2 = 1;
float cir = 0;  // chu vi banh xe
float distance = 0; // quang duong di duoc
float next_time = 0; //next_time khoang thoi gian thuc hien 1 vong
float velocity = 0; // van toc 1 vong quay
float max_velocity = 0;
unsigned long time1, time2; // thoi gian chan va le
#define R 30 // ban kinh banh xe
#define VAL 100 // nguong sensor count
//============== VOID SETUP ========
void setup (){
  pinMode (sensor, INPUT); 
  Serial.begin(115200); 
  // tinh chu vi
  cir = PI*2*R/100; // chia 100 ra met
}
void serial_print(int value){
  Serial.print("turn: ");
  Serial.print(count);
  Serial.print("   distance: ");
  Serial.print(distance);
  Serial.print(" m");
  Serial.print("   speed: ");
  Serial.print(velocity);
 // Serial.print(next_time);
  Serial.print(" m/s");
  Serial.print("   max_speed: ");
  Serial.print(max_velocity);
  Serial.print(" m/s  ");
  Serial.println(value);
}
void distance_com (){ // quang duong di duoc
  distance =  count*cir; // quang duong bang chu vi nhan so vong
}
void velocity_com (){ // van toc xe
  if (next_time == 0) velocity = 0; // khoi dau v = 0
  else
  velocity = cir/next_time;  // van toc 1 vong bang chu vi / thoi gian
  
  if (velocity > max_velocity) max_velocity = velocity; // lay gia tri lon nhat
}
void get_time(){ // lay thoi gian giua 2 vong quay
  if (count % 2 == 1){ 
    if (mutex2 == 1){
     time1 = millis();
     next_time = float((time1 - time2))/1000; // cap nhat toc do 1 --> 2
     mutex2 = 0;
    }
   }else if (count % 2 == 0){
    if (mutex2 == 0){
     time2 = millis(); 
     next_time = float((time2 - time1))/1000; // cap nhat toc do 2 -> 1
     mutex2 = 1;
    }
   }
}
//============== VOID LOOP ========
void loop (){
  int value = analogRead(sensor); // gia tri doc sensor
   if (value < VAL) {
    if (mutex == 1){
      count ++;
      mutex = 0;
    }
   } else mutex = 1;

   
  get_time(); // tinh thoi gian giua 2 lan
  distance_com(); // tinh quang duong
  velocity_com (); // tinh van toc
  serial_print(value); // in gia tri
      
}
