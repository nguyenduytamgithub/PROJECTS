String TEXT = ""; // chuoi nhan thong tin
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  while(Serial.available()) {
      TEXT = Serial.readString();// read the incoming data as string
      Serial.println(TEXT);
    }  
}
