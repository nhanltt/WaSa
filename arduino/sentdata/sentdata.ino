

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

#include <CD74HC4067.h>
//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "AsusFung"
#define WIFI_PASSWORD "9holyd2534"

/* 2. Define the API Key */
#define API_KEY "AIzaSyDDp-TZedDdUaBg4SmiJcN0AQs5LUM1g7k"

/* 3. Define the RTDB URL */
#define DATABASE_URL "helloworld-f400c-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "fpongsapak@gmail.com"
#define USER_PASSWORD "pongsapak"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

CD74HC4067 mux(D1, D2, D3, D4);
const int g_common_pin = A0;
unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup()
{
  pinMode(g_common_pin, INPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.begin(115200);
 
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h



  Firebase.begin(&config, &auth);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);



}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float wind(){
   mux.channel(2);
  float sensorValue = analogRead(g_common_pin);
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
  firebase(sensorValue,voltage,Wind_speed,Speed_mph);
  delay(1000);
  return sensorValue,voltage,Wind_speed,Speed_mph;
 
}
int firebase(float sensorValue,float voltage,float Wind_speed,float Speed_mph){
  int i,val[10];

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
       sendDataPrevMillis = millis();
      
        
           mux.channel(4);
           val[4] = analogRead(g_common_pin);
          Serial.printf("Set int Light  1... %s\n", Firebase.setInt(fbdo, "/Set/Light 1",val[4]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Get int Light  1... %s\n", Firebase.getInt(fbdo, "/Set/Light 1") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
           mux.channel(5);
           val[5] = analogRead(g_common_pin);
          Serial.printf("Set int Light  2... %s\n", Firebase.setInt(fbdo, "/Set/Light 2",val[5]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Get int Light  2... %s\n", Firebase.getInt(fbdo, "/Set/Light 2") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
          mux.channel(6);
          val[6] = analogRead(g_common_pin);
          Serial.printf("Set int Light  3... %s\n", Firebase.setInt(fbdo, "/Set/Light 3",val[6]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Get int Light  3... %s\n", Firebase.getInt(fbdo, "/Set/Light 3") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
           mux.channel(7);
          val[7] = analogRead(g_common_pin);
          Serial.printf("Set int Light  4... %s\n", Firebase.setInt(fbdo, "/Set/Light 4",val[7]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Get int Light  4... %s\n", Firebase.getInt(fbdo, "/Set/Light 4") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
           mux.channel(8);
          val[8] = analogRead(g_common_pin);
          Serial.printf("Set int Light  5... %s\n", Firebase.setInt(fbdo, "/Set/Light 5",val[8]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Get int Light  5... %s\n", Firebase.getInt(fbdo, "/Set/Light 5") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());

          Serial.printf("Set float wind_Speed... %s\n", Firebase.setFloat(fbdo, "/Set/wind_Speed",Wind_speed) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Get float wind_Speed... %s\n", Firebase.getFloat(fbdo, "/Set/wind_Speed") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

          Serial.printf("Set float Speed_mph... %s\n", Firebase.setFloat(fbdo, "/Set/Speed_mph",Speed_mph) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Get float Speed_mph... %s\n", Firebase.getFloat(fbdo, "/Set/Speed_mph") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
          int iVal = 0;
          
     
          Serial.printf("Push int Light 1 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 1", val[4]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push int Light 2 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 2", val[5]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push int Light 3 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 3", val[6]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push int Light 4 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 4", val[7]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push int Light 5 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 5", val[8]) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push float wind_Speed... %s\n", Firebase.pushFloat(fbdo, "/Push/wind_Speed",Wind_speed) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push float Speed_mph... %s\n", Firebase.pushFloat(fbdo, "/Push/Speed_mph", Speed_mph) ? "ok" : fbdo.errorReason().c_str());
    
}
}
void loop()
{

 wind();
 
}
