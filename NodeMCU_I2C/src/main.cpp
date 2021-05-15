#include <Arduino.h>
//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

#include <SoftwareSerial.h>
SoftwareSerial s(D2,D3); //RX,TX

// WiFi network info.
char ssid[] = "SSID";
char wifiPassword[] = "PASSWORD";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "36978690-c8d8-11e6-8a38-e78fd50030b1";
char password[] = "a92407b5cf3009d69b35abd509512f15c8b020ba";
char clientID[] = "b71f0c20-b4ce-11eb-a2e4-b32ea624e442";

//Cayenne Virtual Channel Info
#define TEMP_CHANNEL 0
#define BPM_CHANNEL 1
#define SOUND_CHANNEL 2
#define ROLL_CHANNEL 3

int cel,myBPM,sound,roll;
int temp,bpm,snd,r;
String Read_Buffer;
char* Read_String = NULL;
volatile boolean Receive_Flag_State = false;

int random_value(int min, int max) {
  return min + random(max - min);
}

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
    String readString = "";
    String Data1;
    String Data2;
    String Data3;
    String Data4;
    while(s.available())  {
      char c = s.read();  
      if (c == '^') {
      Serial.println();
      Serial.print("captured String is : "); 
      Serial.println(readString); 
      
      int ind1 = readString.indexOf('|');  
      Data1 = readString.substring(0, ind1);   
      int ind2 = readString.indexOf('|', ind1+1 );  
      Data2 = readString.substring(ind1+1, ind2);  
      int ind3 = readString.indexOf('|', ind2+1 );
      Data3 = readString.substring(ind2+1, ind3);
      int ind4 = readString.indexOf('^', ind3+1 );
      Data4 = readString.substring(ind3+1);

      // Serial.print("cel");
      // Serial.println(Data1); 
      // Serial.print("myBPM");
      // Serial.println(Data2);
      // Serial.print("sound");
      // Serial.println(Data3);
      // Serial.print("roll");
      // Serial.println(Data4);
      // Serial.println();
      // Serial.println();
      
    cel = Data1.toInt();
    myBPM =Data2.toInt();
    sound = Data3.toInt();
    roll = Data4.toInt();
      readString=""; 
      Data1 = "";
      Data2 = "";
      Data3 = "";
      Data4 = "";
    }  
    else {     
      readString += c; 
    }
  }


    //Serial.println(readString);
    Serial.print(cel); Serial.print("\t");
    Serial.print(myBPM); Serial.print("\t");
    Serial.print(sound); Serial.print("\t");
    Serial.println(roll);

  Cayenne.loop();
}
CAYENNE_OUT_DEFAULT()
{
	Cayenne.celsiusWrite(TEMP_CHANNEL, cel);
	Cayenne.virtualWrite(BPM_CHANNEL, myBPM, "Heart_Rate", "bpm");
	Cayenne.virtualWrite(SOUND_CHANNEL, sound, "Sound_Sensor", "dB");
  Cayenne.virtualWrite(ROLL_CHANNEL, roll, "Gyro_Sensor", "Deg °");
}
