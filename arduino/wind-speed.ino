#include <CD74HC4067.h>

               // s0 s1 s2 s3
CD74HC4067 mux(D1, D2, D3, D4);
const int g_common_pin = A0; 
void setup()
{
    pinMode(g_common_pin, INPUT); // set the initial mode of the common pin.
                                 // This can be changed in loop() for for each channel.
    Serial.begin(9600);
    
}
void loop()
{
  mux.channel(2);
  float sensorValue = analogRead(A0);
  Serial.print("Analog Value =");
  Serial.println(sensorValue);
 
  float voltage = (sensorValue / 1023) * 5;
  Serial.print("Voltage =");
  Serial.print(voltage);
  Serial.println(" V");
 
  float wind_speed = mapfloat(voltage, 0.4, 2, 0, 32.4);
  float speed_mph = ((wind_speed *3600)/1609.344);
  Serial.print("Wind Speed =");
  Serial.print(wind_speed);
  Serial.println("m/s");
  delay(1000);
  Serial.print(speed_mph);
  Serial.println("mph");
  delay(1000);
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
  
