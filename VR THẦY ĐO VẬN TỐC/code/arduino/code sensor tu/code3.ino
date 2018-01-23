int min_val, max_val;
int count = 0;
int x = 1;
float w_length = 2.050;
volatile float SPEED;
unsigned long timer, timer2;
void setup() {
  pinMode(A0,INPUT);
  Serial.begin(115200);
  pinMode(3,OUTPUT);
  digitalWrite(3,1);
}

void loop() {
  if (SPEED > 30) {   
    SPEED = 0;  
   }
   if ((millis() - timer2) > 2000) {     
     SPEED = 0; 
   }
  if (millis() - timer > 20){
   if (analogRead(A0) > max_val) max_val = analogRead(A0);
      Serial.println(SPEED);
//      Serial.print ("  ");
//      Serial.println (count);
      if (analogRead(A0) > (max_val-10)){
         
         SPEED = w_length / ((float)(millis() - timer) / 1000) ; 
         timer = millis();
         if (x == 1){
           count ++;
           x =0;
         }
      }else x = 1;
  }
  timer2 = millis();
}
