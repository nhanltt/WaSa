const int a1 = A0;
const int a2 = A1;
const int a3 = A2;
const int a4 = A3;
const int a5 = A4;
const int a6 = A5;
#include <SoftwareSerial.h>
#include <string.h>
SoftwareSerial UnoSerial(3, 2); // RX | TX
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup()
{
  pinMode(3, INPUT);
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  UnoSerial.begin(57600);
  pinMode(a1, INPUT);
  pinMode(a2, INPUT);
  pinMode(a3, INPUT);
  pinMode(a4, INPUT);
  pinMode(a5, INPUT);
  pinMode(a6, INPUT);
 
 
}

void loop() {
     
     int i,k,light[10],val[2];
     char a[10][10]={"a1","a2","a3","a4","a5","a6"};
     /*int  darkness = 1000 ,brightness= 0;
     int light_id;*/
        
     
    for (i=0;i<=4;i++){
      light[i] = analogRead(a[i]);
      Serial.print("Ldr＿");
      Serial.print(i+1);
      Serial.print(":");
      Serial.println(light[i]);
   
      delay(500);
    } 
   


      
      
   
   float sensorValue = analogRead(a6);
  Serial.print("Analog Value =");
  Serial.println(sensorValue);
 
  float voltage = (sensorValue / 1023) * 5;
  Serial.print("Voltage =");
  Serial.print(voltage);
  Serial.println(" V");
 
  float Wind_speed = mapfloat(voltage, 0.4, 2, 0, 32.4);
  float Speed_mph = ((Wind_speed *3600)/1609.344);
  Serial.print("Wind Speed =");
  Serial.print(Wind_speed);
  Serial.println("m/s");
  delay(1000);
  Serial.print(Speed_mph);
  Serial.println("mph");

  for(i=0;i<=4;i++){
   
       UnoSerial.print(light[i]);
       UnoSerial.print(" ");
  }   
       UnoSerial.print(Wind_speed);
       UnoSerial.print(" ");
       UnoSerial.print(Speed_mph);
       UnoSerial.print("\n");
      
      

}
