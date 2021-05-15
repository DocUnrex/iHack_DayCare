#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>  
#include <SoftwareSerial.h>
SoftwareSerial s(5,6); //Rx,Tx
 
const int PulseWire = A0;
int soundPin = A1;
int tempPin =A2;
int rollpin=A3;       
const int LED16 = 12;          
int Threshold =50;
int val;
int y,a,b,sound,myBPM,cel;        
PulseSensorPlayground pulseSensor; 

char Temp_Data[100];

int random_value(int min, int max) {
  return min + random(max - min);
}

void setup() {  
  s.begin(9600);
  Serial.begin(9600);  
  y=analogRead(rollpin);         
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED16);       
  pulseSensor.setThreshold(Threshold); 
  pinMode(soundPin,INPUT);   
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  
}
void loop() {
  val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000;
  cel = int(mv/20);
  myBPM = pulseSensor.getBeatsPerMinute();  // To keep atrack of baby's pulse
  if (pulseSensor.sawStartOfBeat()) {
    Serial.println("♥   HeartBeat Happened ! ");
    if(myBPM<70){
      Serial.println("♥  HeartBeat decreased ");
      Serial.println(myBPM);
      }
    if(myBPM>140){
      Serial.println("♥  HeartBeat Increased ");
      Serial.println(myBPM);
      }
    }
  // To check baby is having fever or hypothermia
   if(cel>=38){
    Serial.println("Baby is having Fever");
    Serial.println(cel);
    }
   if(cel<36){
    Serial.println("Baby is having Hypothermia");
    Serial.println(cel);
   }
   Serial.println(cel);
   //To check if baby is crying
  sound=analogRead(soundPin);
  if(sound>70 && sound<80){
    Serial.println("BAby is crying");
    digitalWrite(LED16,HIGH);
    delay(1000);
    digitalWrite(LED16,LOW);
     delay(1000);
  } 
  //To check if baby rolled over
  a=analogRead(rollpin);
  b=a-y;
  //Serial.print("\n");
  delay(90);
  if (b>90){
    Serial.println("Baby rolled over"); 
  }
  else if(b<-90){
    Serial.println("Baby rolled over "); 
  }
   delay(1000);
   
    String d;
    d = String(cel)+ "|" + String(myBPM)+ "|" + String(sound)+ "|" + String(a)+ "^";
    d.toCharArray(Temp_Data, sizeof(Temp_Data));
    //s.write(Temp_Data);
    if(s.available()>0)
    {
      s.write(Temp_Data,sizeof(Temp_Data));  
    }
     Serial.println(d);
}


