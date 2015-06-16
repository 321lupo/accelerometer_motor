
  #include<Wire.h>
  #include <MIDI.h>
  const int channel = 1;         //MIDI CHANNEL
  
  #define accel_module (0x53)    //ACCELEROMETER SETUP
  byte values [6] ;
  char output [512];

  const int motorPin = 3;

void setup() {
  
  Serial.begin(9600);
  
  Wire.begin();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(16);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  
  pinMode(motorPin, OUTPUT);
}
  int accelMidiX;
  int accelMidiY;
  int accelMidiXOld;
  int accelMidiYOld;

void loop() {
  
   int16_t xyzregister = 0x32;
   int16_t x, y, z;
   Wire.beginTransmission(accel_module);
   Wire.write(xyzregister);
   Wire.endTransmission();
   Wire.beginTransmission(accel_module);
   Wire.write(xyzregister);
   Wire.endTransmission();
   Wire.beginTransmission(accel_module);
   Wire.requestFrom(accel_module, 6);
   int16_t i = 0;
   while(Wire.available()){
     values[i] = Wire.read();
     i++;
   }
   Wire.endTransmission();
   
   x = (((int16_t)values[1]) << 8) | values [0];
   y = (((int16_t)values[3]) << 8) | values [2];
   z = (((int16_t)values[5]) << 8) | values [4];
  
   
  /*if (x>130) x=130;
  if (x<-130) x=-130;
  if (y>130) y=130;
  if (y<-130) y=-130;*/
  
  if (z<=-300) {
  digitalWrite(motorPin, HIGH);
  delay(500);
  digitalWrite(motorPin, LOW);
  }
  
  if (abs(accelMidiX-accelMidiXOld) > 5){
    int accelMidiX = map (x, -130, 130, 0, 127);
    int accelMidiXOld = accelMidiX;
    usbMIDI.sendControlChange(100, accelMidiX, channel); 
  }
  if (abs(accelMidiY-accelMidiYOld) > 5){
    int accelMidiY = map (y, 130, -130, 0, 127);
    int accelMidiYOld = accelMidiY;
    usbMIDI.sendControlChange(101, accelMidiY, channel);
  }
   
   
   sprintf(output, "%d %d %d", x, y, z);
   Serial.print(output);
   Serial.write(10);
   
   while (usbMIDI.read()) ; // read and discard any incoming MIDI messages
   
   delay(200);

}
