#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <FirebaseESP8266.h>
#endif

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "AsusFung"
#define WIFI_PASSWORD "9holyd2534"

/* 2. Define the API Key */
#define API_KEY "AIzaSyDDp-TZedDdUaBg4SmiJcN0AQs5LUM1g7k"


/* 3. Define the RTDB URL */
#define DATABASE_URL "helloworld-f400c-default-rtdb.asia-southeast1.firebasedatabase.app/" 

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "fpongsapak@gmail.com"
#define USER_PASSWORD "pongsapak"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

SoftwareSerial NodeSerial(D2, D3); // RX | TX
void setup() {
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(9600);
  NodeSerial.begin(57600);
  Serial.println();
  Serial.println("NodeMCU/ESP8266 Run");

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

  

void loop() {
    while (NodeSerial.available() > 0)
  {
     int light = NodeSerial.parseInt();
     int light1 = NodeSerial.parseInt();
     int light2 = NodeSerial.parseInt();
     int light3 = NodeSerial.parseInt();
     int light4 = NodeSerial.parseInt();
     float Wind_speed = NodeSerial.parseFloat();
     float Speed_mph =  NodeSerial.parseFloat();
     
    if (NodeSerial.read() == '\n')
    {
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
       {
       sendDataPrevMillis = millis();
      
      Serial.print("NodeMCU or ESP8266");
      Serial.println(" : ");
      Serial.print("Ldr 1:");
      Serial.println(light);
      Serial.printf("Set int Light  1... %s\n", Firebase.setInt(fbdo, "/Set/Light 1",light) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int Light  1... %s\n", Firebase.getInt(fbdo, "/Set/Light 1") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
      
      Serial.print("Ldr 2:");
      Serial.println(light1);
      Serial.printf("Set int Light  2... %s\n", Firebase.setInt(fbdo, "/Set/Light 2",light1) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int Light  2... %s\n", Firebase.getInt(fbdo, "/Set/Light 2") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
      
      Serial.print("Ldr 3:");
      Serial.println(light2);
      Serial.printf("Set int Light  3... %s\n", Firebase.setInt(fbdo, "/Set/Light 3",light2) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int Light  3... %s\n", Firebase.getInt(fbdo, "/Set/Light 3") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
         
      Serial.print("Ldr 4:");
      Serial.println(light3);
      Serial.printf("Set int Light  4... %s\n", Firebase.setInt(fbdo, "/Set/Light 4",light3) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int Light  4... %s\n", Firebase.getInt(fbdo, "/Set/Light 4") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
      
      Serial.print("Ldr 5:");
      Serial.println(light4);
      Serial.printf("Set int Light  5... %s\n", Firebase.setInt(fbdo, "/Set/Light 5",light4) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int Light  5... %s\n", Firebase.getInt(fbdo, "/Set/Light 5") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
      
      Serial.print("Wind Speed =");
      Serial.print(Wind_speed);
      Serial.println("m/s");
      Serial.printf("Set float wind_Speed... %s\n", Firebase.setFloat(fbdo, "/Set/wind_Speed",Wind_speed) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get float wind_Speed... %s\n", Firebase.getFloat(fbdo, "/Set/wind_Speed") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
      
      Serial.print(Speed_mph);
      Serial.println("mph");
      
      Serial.printf("Set float Speed_mph... %s\n", Firebase.setFloat(fbdo, "/Set/Speed_mph",Speed_mph) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get float Speed_mph... %s\n", Firebase.getFloat(fbdo, "/Set/Speed_mph") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());   
         
   
     int iVal = 0;
          
     
          /*Serial.printf("Push int Light 1 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 1",light) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push int Light 2 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 2",light1 ) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push int Light 3 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 3",light2 ) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push int Light 4 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 4",light3 ) ? "ok" : fbdo.errorReason().c_str());
          Serial.printf("Push int Light 5 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 5",light4 ) ? "ok" : fbdo.errorReason().c_str());*/
  }
     
    }
}
}
