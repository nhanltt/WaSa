const int a1 = A0;
const int a2 = A1;
const int a3 = A2;
const int a4 = A3;
const int a5 = A4;
const int a6 = A5;
#include <SoftwareSerial.h>
SoftwareSerial UnoSerial(3, 2); // RX | TX
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float wind(){
 
  

 
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
 
     int light = analogRead(a1);
      Serial.print("Ldr 1:");
      Serial.println(light);
     

      int light1 = analogRead(a2);
      Serial.print("Ldr 2:");
      Serial.println(light1);
      delay(200);
      int light2  = analogRead(a3);
      Serial.print("Ldr 3:");
      Serial.println(light2);
     delay(200);
      int light3 = analogRead(a4);
      Serial.print("Ldr 4:");
      Serial.println(light3);
    delay(200);
      int light4= analogRead(a5);
      Serial.print("Ldr 5:");
      Serial.println(light4);  
   
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
  
  

       UnoSerial.print(light);
       UnoSerial.print(" ");
       
       UnoSerial.print(light1);
       UnoSerial.print(" ");
       
       UnoSerial.print(light2);
       UnoSerial.print(" ");
       
       UnoSerial.print(light3);
       UnoSerial.print(" ");
        
       UnoSerial.print(light4);
       UnoSerial.print(" ");
       UnoSerial.print(Wind_speed);
       UnoSerial.print(" ");
       
       UnoSerial.print(Speed_mph);
       UnoSerial.print("\n");
      

}
