
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>



#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
// include ไลบรารีสำหรับรับค่าเวลา
#include <NTPClient.h>
#include <WiFiUdp.h>



// include ไลบรารีสำหรับรับค่าเวลา
#include <NTPClient.h>
#include <WiFiUdp.h>

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

unsigned long epochTime; 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

unsigned long sendDataPrevMillis = 0;


unsigned long count = 0;

unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}


//The resets by whatchdog STOPS!!
SoftwareSerial NodeSerial(D2, D3); // RX | TX
void setup() {
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  timeClient.begin();
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
  //ESP.wdtDisable(); 
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
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
   ArduinoOTA.handle();
     while (NodeSerial.available() > 0)
  {  int light[10];
      light[0] = NodeSerial.parseInt();
      light[1] = NodeSerial.parseInt();
      light[2] = NodeSerial.parseInt();
      light[3] = NodeSerial.parseInt();
      light[4] = NodeSerial.parseInt();
      float Wind_speed = NodeSerial.parseFloat();
     
       if (Wind_speed <= 0 ){
        Wind_speed = 0;
      }
       
     float Speed_mph =  NodeSerial.parseFloat();
     epochTime = getTime();
      //float Wind_speed = random(0, 240);
     float randWind_direc = random(0, 15),mn_wind,mx_wind;
     int  darkness =0 ,brightness=1000;
     int light_id, min_brightness, max_brightness;
    if (NodeSerial.read() == '\n')
    {
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
       {
       sendDataPrevMillis = millis();
      
        Serial.print("NodeMCU or ESP8266");
        Serial.println(" : ");
        Serial.print(light_id);
         
    for (int i=0;i<=4;i++){
      Serial.print("Ldr＿");
      Serial.print(i+1);
      Serial.print(":");
      Serial.println(light[i]);
    
       /*if (light[i] > darkness ) {
         darkness = light[i];
         light_id[0] = i+1;
       }*/
      if (light[i] < brightness) {
         brightness = light[i];
        light_id = i+1;
       
      }
      
    }
         Serial.print("The darkness of the array is: ");
      Serial.println(darkness);
      Serial.println(light_id);
   
      Serial.print("The brightness of the array is: ");
      Serial.println(brightness);
   
        Serial.printf("Set int Light  1... %s\n", Firebase.setInt(fbdo, "/Set/Light 1",light[0]) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get int Light  1... %s\n", Firebase.getInt(fbdo, "/Set/Light 1") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set int Light  2... %s\n", Firebase.setInt(fbdo, "/Set/Light 2",light[1]) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get int Light  2... %s\n", Firebase.getInt(fbdo, "/Set/Light 2") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set int Light  3... %s\n", Firebase.setInt(fbdo, "/Set/Light 3",light[2]) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get int Light  3... %s\n", Firebase.getInt(fbdo, "/Set/Light 3") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set int Light  4... %s\n", Firebase.setInt(fbdo, "/Set/Light 4",light[3]) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get int Light  4... %s\n", Firebase.getInt(fbdo, "/Set/Light 4") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set int Light  5... %s\n", Firebase.setInt(fbdo, "/Set/Light 5",light[4]) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get int Light  5... %s\n", Firebase.getInt(fbdo, "/Set/Light 5") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set brightness value... %s\n", Firebase.setInt(fbdo, "/Set/sun_direction/brightness",brightness) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get brightness value... %s\n", Firebase.getInt(fbdo, "/Set/sun_direction/brightness") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set light_id value... %s\n", Firebase.setInt(fbdo, "/Set/sun_direction/light_id",light_id) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get light_id value... %s\n", Firebase.getInt(fbdo, "/Set/sun_direction/light_id") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set int Epoch Time... %s\n", Firebase.setInt(fbdo, "/Set/EpochTime", epochTime) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get int Epoch Time... %s\n", Firebase.getInt(fbdo, "/Set/EpochTime") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set float wind_Speed... %s\n", Firebase.setFloat(fbdo, "/Set/wind_Speed",Wind_speed) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get float wind_Speed... %s\n", Firebase.getFloat(fbdo, "/Set/wind_Speed") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());
        Serial.printf("Set float Speed_mph... %s\n", Firebase.setFloat(fbdo, "/Set/Speed_mph",Speed_mph) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get float Speed_mph... %s\n", Firebase.getFloat(fbdo, "/Set/Speed_mph") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());  
        Serial.printf("Set float Wind_Direction... %s\n", Firebase.setFloat(fbdo, "/Set/Wind_Direction",randWind_direc) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get float Wind_Direction... %s\n", Firebase.getFloat(fbdo, "/Set/WInd_Direction") ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str()); 
        
        Serial.printf("Push int Light 1 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 1",light[0]) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push int Light 2 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 2",light[1] ) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push int Light 3 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 3",light[2] ) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push int Light 4 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 4",light[3] ) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push int Light 5 ... %s\n", Firebase.pushInt(fbdo, "/Push/Light 5",light[4] ) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push brightness ... \n", Firebase.pushInt(fbdo, "/Push/sun_direction/brightness",brightness) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push light_id ... \n", Firebase.pushInt(fbdo, "/Push/sun_direction/light_id",light_id) ? "ok" : fbdo.errorReason().c_str());    
        Serial.printf("Push int Epoch Time... %s\n", Firebase.pushInt(fbdo, "/Push/EpochTime", epochTime) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push float wind_Speed... %s\n", Firebase.pushFloat(fbdo, "/Push/wind_Speed",Wind_speed) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push float Speed_mph... %s\n", Firebase.pushFloat(fbdo, "/Push/Speed_mph", Speed_mph) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Push float Wind_Direction... %s\n", Firebase.pushFloat(fbdo, "/Push/Wind_Direction", randWind_direc) ? "ok" : fbdo.errorReason().c_str());
        
     if (Firebase.RTDB.getInt(&fbdo, "/Set/min/brightness")) {
      if (fbdo.dataType() == "int") {
           min_brightness = fbdo.intData();
           Serial.print("min_brightness:");
           Serial.println(min_brightness);
        if (brightness < min_brightness){
            Serial.printf("Set min brightness value... %s\n", Firebase.setInt(fbdo, "/Set/min/brightness",brightness) ? "ok" : fbdo.errorReason().c_str());
         }
       }
     }
     else  {
       Serial.println(fbdo.errorReason());
     }
     
     if (Firebase.RTDB.getFloat(&fbdo, "/Set/min/wind_Speed")) {
       
     if ((fbdo.dataType() ==  "float" )||(fbdo.dataType() ==  "int")) {
          
           mn_wind = fbdo.floatData();
           Serial.print("min_windspeed :");
           Serial.println(mn_wind);
        if (Wind_speed < mn_wind){
           Serial.printf("Set float min wind_Speed... %s\n", Firebase.setFloat(fbdo, "/Set/min/wind_Speed",Wind_speed) ? "ok" : fbdo.errorReason().c_str());
         }
       }
     }
     else  {
       Serial.println(fbdo.errorReason());
     }
     
     if (Firebase.RTDB.getInt(&fbdo, "/Set/max/brightness")) {
      if (fbdo.dataType() == "int") {
           max_brightness = fbdo.intData();
           Serial.print("max_brightness:");
           Serial.println(max_brightness);
        if (brightness > max_brightness){
            Serial.printf("Set max brightness value... %s\n", Firebase.setInt(fbdo, "/Set/max/brightness",brightness) ? "ok" : fbdo.errorReason().c_str());
         }
       }
     }
     else  {
       Serial.println(fbdo.errorReason());
     }
     
     if (Firebase.RTDB.getFloat(&fbdo, "/Set/max/wind_Speed")) {
            
      if ((fbdo.dataType() ==  "float" )||(fbdo.dataType() ==  "int")) {
       
           mx_wind = fbdo.floatData();
           
           Serial.print("max_windspeed :");
           Serial.println(mx_wind);
          if (Wind_speed > mx_wind){
           Serial.printf("Set max float wind_Speed... %s\n", Firebase.setFloat(fbdo, "/Set/max/wind_Speed",Wind_speed) ? "ok" : fbdo.errorReason().c_str());
         }
       }
     }
     else  {
       Serial.println(fbdo.errorReason());
     }
    delay(3000);
          count++; 
     
    }
}

}
}
     
         
