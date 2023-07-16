#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <time.h>
#include <ArduinoJson.h>

const char* ssid = "Dego";
const char* password = "orit1212";

const int relayPin = D4;  // פין המחובר לרלהי
const int lightSensorPin = A0;  // פין המחובר לחיישן האור
const int ledPin = D4;  // פין המחובר ללד
const unsigned long step1Duration = 1000;  // משך השלב 1 במילישניות
const unsigned long step2Duration = 1000;  // משך השלב 2 במילישניות
const unsigned long step3Duration = 1000;  // משך השלב 3 במילישניות
const unsigned long step4Duration = 1000;  // משך השלב 4 במילישניות
int timezone = 1 * 3600;   // החלף "x" עם אזור הזמן שלך
int time_dst = 0;

unsigned long stepStartTime = 0;
int pwmSum = 0;
int pwmCount = 0;

// החלף עם פרטי השרת MongoDB שלך
const char* mongodbServer = "mongodb+srv://yosefdego:Yd5668854@yosefdego12.sbzfv5z.mongodb.net/test:";  // כתובת השרת MongoDB
const int mongodbPort = 8000;  // הפורט של השרת MongoDB
const char* mongodbDatabase = "test";  // שם מסד הנתונים בשרת MongoDB
const char* mongodbCollection = "time_managments";  // שם האוסף במסד הנתונים

WiFiClient wifiClient;  // לקוח WiFi

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
   
  Serial.begin(9600);
  
  configTime(timezone, time_dst, "pool.ntp.org","time.nist.gov");//מקבל את אזור הזמן מהשרת
}

void loop() {
  
  if (wifiClient.connect(mongodbServer, mongodbPort)) {
    Serial.println("מחובר לשרת MongoDB");

    
    String url = "/getleveltime/ + prossec";

   
    HTTPClient httpClient;
    httpClient.begin(wifiClient, "http://192.168.1.44:8000/getleveltime" + url);
    int httpCode = httpClient.GET();

   
    if (httpCode == HTTP_CODE_OK) {
      
      String response = httpClient.getString();
      DynamicJsonDocument jsonDocument(1024);
      deserializeJson(jsonDocument, response);

      
      int heatingTime = jsonDocument["heatingTime"].as<int>();
      int lightTestTime = jsonDocument["lightTestTime"].as<int>();
      int sophisticatedComponentTime = jsonDocument["sophisticatedComponentTime"].as<int>();
      int delayTime = jsonDocument["delayTime"].as<int>();
      
      
      Serial.print("זמן חימום: ");
      Serial.println(heatingTime);
      Serial.print("זמן בדיקת אור: ");
      Serial.println(lightTestTime);
      Serial.print("זמן רכיב מתקדם: ");
      Serial.println(sophisticatedComponentTime);
      Serial.print("זמן השהייה: ");
      Serial.println(delayTime);
    } else {
      Serial.print("נכשל בשליפת הנתונים. קוד שגיאה: ");
      Serial.println(httpCode);
    }

   
    httpClient.end();
    wifiClient.stop();
  } else {
    Serial.println("נכשל בהתחברות לשרת MongoDB");
  }
  
 
  delay(2000);

  unsigned long currentTime = millis();
  
 
  startStep(1);
  
  // שלב 1: הדלקת הרלהי
  if (currentTime - stepStartTime >= step1Duration) {
    digitalWrite(relayPin, LOW);
    startStep(2);
  }

  // שלב 2: מדידת ערך האור דרך חיישן האור
  if (currentTime - stepStartTime >= step2Duration) {
    int lightValue = analogRead(lightSensorPin);
    
    // חישוב ערך ממוצע של PWM
    pwmSum += map(lightValue, 0, 1023, 0, 255);
    pwmCount++;
    
    // שלב 3: הדלקת הלד עם ערך ממוצע של PWM
    analogWrite(ledPin, pwmSum / pwmCount);
    
    startStep(3);
  }
  
  // שלב 3: כיבוי הלד
  if (currentTime - stepStartTime >= step3Duration) {
    analogWrite(ledPin, 0);
    
    startStep(4);
  }
  

// שלב 4: השהיית התהליך לזמן מסויים
if (currentTime - stepStartTime >= step4Duration) {
startStep(1);
}
}

void startStep(int step) {
stepStartTime = millis();

switch (step) {
case 1:
Serial.println("שלב 1: הדלקת הרלהי");
digitalWrite(relayPin, HIGH);
break;

case 2:
  Serial.println("שלב 2: מדידת ערך האור דרך חיישן האור");
  pwmSum = 0;
  pwmCount = 0;
  break;

case 3:
  Serial.println("שלב 3: הדלקת הלד");
  break;

case 4:
  Serial.println("שלב 4: השהיית התהליך");
  break;

  }
}
