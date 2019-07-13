/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "Samsung Galaxy S7 8574"
#define STAPSK  "auhq4212"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // wait for WiFi connection

  
  WiFiClient client;
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");

  if(http.begin(client, "http://192.168.43.225:8000/sensors/send_data/"))
  {
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
    Serial.print("[HTTP] POST...\n");
    String payload = "{\"temp_soil\": \"11.11\",\"temp_room\": \"22.22\",\"humidity\": \"33.33\",\"moisture\": \"44.44\",\"lux\": \"55.55\"}";
    int httpCode = http.POST(payload);
    Serial.print("httpCode: ");
    Serial.print(httpCode);
  }
  /*
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://jigsaw.w3.org/HTTP/connection.html")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
*/
  delay(10000);
}
