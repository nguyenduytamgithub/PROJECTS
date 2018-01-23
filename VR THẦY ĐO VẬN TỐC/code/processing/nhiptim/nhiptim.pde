import processing.serial.*;
 
final float FLAT_LINE=512;
final float MINVALUE=0;
final float MAXVALUE=1000;
 
 
PImage ecg;
int xPos = 1;         // horizontal position of the graph
 
float[] ecgData;
Serial myPort;        // The serial port
 
void setup () {
  // set the window size:
  size(1000, 400);        
 
  // List all the available serial ports
  println(Serial.list());
  // Open whatever port is the one you're using.
  myPort = new Serial(this, "COM9", 9600);
  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
  // set inital background:
  background(238, 223, 204);
  ecg=loadImage("ECG-full (1).jpg");
  ecgData=new float[width];
 
  //Init data to flat line
  for (int i=0; i<width; i++)
    ecgData[i]=FLAT_LINE;

  stroke(0);
}
 
void draw () {
  // everything happens in the serialEvent()
  background(0xff);
  //Draw all data
  for (int i=0; i<width-1; i++)
    line(i, ecgData[i], i+1, ecgData[i+1]);
}
 
void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  float inByte = 0;
 
  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
 
    // If leads off detection is true notify with blue line
    if (inString.equals("!")) { 
      stroke(#390606); //Set stroke to blue ( R, G, B)
      inByte = FLAT_LINE;  // middle of the ADC range (Flat Line)
    }
    // If the data is good let it through
    else {
      stroke(0xff, 50, 0); //Set stroke to red ( R, G, B)
      inByte = float(inString);
    }
 
    ecgData[xPos]=map(inByte, MINVALUE, MAXVALUE, height, 0);
    xPos=(xPos+1)%width;   //Ensure value is increased and limited into the range of 0 to width
  }
}