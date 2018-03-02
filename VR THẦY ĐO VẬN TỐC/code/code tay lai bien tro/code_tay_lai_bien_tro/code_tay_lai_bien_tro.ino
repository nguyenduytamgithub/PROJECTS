#define MAX 20
#define MIN 0
void setup (){
 Serial.begin(115200);
  }
 void loop (){
 int val = analogRead(A0);
 int vec = map(val,0,1023,MIN,MAX);
 Serial.println(vec);
 }
