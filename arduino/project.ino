/*
 * Controlling and looping through a CD74HC4067's channel outputs
 *
 *
 * Connect the four control pins to any unused digital or analog pins.
 * This example uses digital pins 4, 5, 6, and 7.
 *
 * Connect the common pin to any other available pin. This is the pin that will be
 * shared between the 16 channels of the CD74HC4067. The 16 channels will inherit the
 * capabilities of the common pin. For example, if it's connected to an analog pin,
 * you will be able to use analogRead on each of the 16 channels.
 *
*/

#include <CD74HC4067.h>

               // s0 s1 s2 s3
CD74HC4067 mux(D1, D2, D3, D4);  // create a new CD74HC4067 object with its four control pins

const int g_common_pin = A0; // select a pin to share with the 16 channels of the CD74HC4067

void setup()
{
    pinMode(g_common_pin, INPUT); // set the initial mode of the common pin.
	                               // This can be changed in loop() for for each channel.
    Serial.begin(9600);
    
}

void loop()
{     
      int val[5];
      mux.channel(1);
      val[1] = analogRead(g_common_pin);
      Serial.print("Ldr 1:");
      Serial.println(val[1]);
      delay(1500);
      
       mux.channel(2);
       val[2] = analogRead(g_common_pin);
      Serial.print("Ldr 2:");
      Serial.println(val[2]);
         delay(1500);
         
       mux.channel(3);
       val[3] = analogRead(g_common_pin);
      Serial.print("Ldr 3:");
      Serial.println(val[3]);
         delay(1500);
         
       mux.channel(4);
       val[4] = analogRead(g_common_pin);
      Serial.print("Ldr 4:");
      Serial.println(val[4]);
         delay(1500);
         
     mux.channel(5);
       val[5] = analogRead(g_common_pin);
      Serial.print("Ldr 5:");
      Serial.println(val[5]);
         delay(1500);

   /* for (int i = 1; i < 6; i++) {
        mux.channel(i);
        int val = analogRead(g_common_pin);
        Serial.print("Ldr:");
        Serial.println(val);
        delay(1000);
        
    }
    */
}
