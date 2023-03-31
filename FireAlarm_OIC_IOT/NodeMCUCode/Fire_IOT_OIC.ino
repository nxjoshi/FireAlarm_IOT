#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <base64.h>
#include <HttpClient.h>


int LED = D2;
int RELAY = D4;
int Flame_sensor = D1;
int Flame_detected;
const char* OICServer = "http://192.168.1.50:3000/IOTServer";
const int sensorMin = 0;     //  sensor minimum
const int sensorMax = 1024;  // sensor maximum
const char* ssid = "Airtel_8454899945";
const char* password = "air47214";

//const char* ssid = "Nitish's iPhone";
//const char* password = "123456789";
ESP8266WebServer server(80); //Server on port 80

void setup()
{
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);    
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


   //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  
 server.begin();                  //Start server
}
void loop() {
  int sensorReading  = analogRead(A0);
  int range = map(sensorReading,  sensorMin, sensorMax, 0, 2);
 Serial.println("Rangeis:");
 Serial.print(range);
 StaticJsonDocument<200> doc;
  // range value:
  switch (range) {
  case 0:    // A fire closer than 1.5 feet away.
     doc["FireStatus"] = "Close";
    Serial.println("** Close  Fire **");
     if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, OICServer);
           http.addHeader("Content-Type", "application/json");
      http.addHeader("Content-Length" ,"52098");
      char out[200];
      Serial.println( serializeJson(doc, out));
      String httpRequestData =   out   ;        
      int httpResponseCode = http.POST(httpRequestData);
      Serial.println(httpRequestData);
      Serial.print("HTTP Response code is: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    break;
  case 1:    // A fire between 1-3 feet away.
   Serial.println(sensorReading);
    Serial.println("**  Distant Fire **");

     doc["FireStatus"] = "Distant";
     if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, OICServer);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("Content-Length" ,"52098");
      char out[200];
      serializeJson(doc, out);
      Serial.println(out );
      String httpRequestData =   out   ; 
     // Serial.println(httpRequestData);    
      int httpResponseCode = http.POST(httpRequestData);
      Serial.println(httpRequestData);
      Serial.print("HTTP Response code is: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    break;
  case 2:    // No fire detected.
  
    Serial.println(sensorReading);
    Serial.println("No  Fire");
    break;
  }
  delay(5000);  // delay between reads
}
