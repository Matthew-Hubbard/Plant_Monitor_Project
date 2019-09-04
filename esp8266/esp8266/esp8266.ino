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

  // Serial.begin(115200);
  // Serial.setDebugOutput(true);

  //Serial.begin(115200);
  Serial.begin(9600);
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

  String sensor_msg = "";
  boolean msg_ready = false;
  
  WiFiClient client;
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  Serial.print("Connecting to 192.168.43.225 with http...\n");
  if(http.begin(client, "http://192.168.43.225:8000/sensors/send_data/"))
  {
    http.addHeader("Content-Type", "multipart/form-data; boundary=boundary");
    
    //String payload = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"sensor_id\"\r\n\r\n7\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"temp_soil\"\r\n\r\n11.11\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"temp_room\"\r\n\r\n22.22\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"humidity\"\r\n\r\n33.33\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"heat_index\"\r\n\r\n44.44\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"moisture\"\r\n\r\n55.55\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"lux\"\r\n\r\n66.66\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"visible\"\r\n\r\n777\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"ir\"\r\n\r\n888\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"full\"\r\n\r\n999\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--";
    //String payload = "--boundary\r\nContent-Disposition: form-data; name=\"sensor_id\"\r\n\r\n7\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_soil\"\r\n\r\n11.11\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_room\"\r\n\r\n22.22\r\n--boundary\r\nContent-Disposition: form-data; name=\"humidity\"\r\n\r\n33.33\r\n--boundary\r\nContent-Disposition: form-data; name=\"heat_index\"\r\n\r\n44.44\r\n--boundary\r\nContent-Disposition: form-data; name=\"moisture\"\r\n\r\n55.55\r\n--boundary\r\nContent-Disposition: form-data; name=\"lux\"\r\n\r\n66.66\r\n--boundary\r\nContent-Disposition: form-data; name=\"visible\"\r\n\r\n777\r\n--boundary\r\nContent-Disposition: form-data; name=\"ir\"\r\n\r\n888\r\n--boundary\r\nContent-Disposition: form-data; name=\"full\"\r\n\r\n999\r\n--boundary--";

    // Get sensor data from serial connection
    sensor_msg = "";
    while(Serial.available()){
      sensor_msg = Serial.readString();
      msg_ready = true;
    }

    String payload = "--boundary\r\nContent-Disposition: form-data; name=\"sensor_id\"\r\n\r\n";
    if(msg_ready)
    {
      Serial.print("[HTTP] POST...\n");
      payload += sensor_msg;
      payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_soil\"\r\n\r\n11.11\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_room\"\r\n\r\n22.22\r\n--boundary\r\nContent-Disposition: form-data; name=\"humidity\"\r\n\r\n33.33\r\n--boundary\r\nContent-Disposition: form-data; name=\"heat_index\"\r\n\r\n44.44\r\n--boundary\r\nContent-Disposition: form-data; name=\"moisture\"\r\n\r\n55.55\r\n--boundary\r\nContent-Disposition: form-data; name=\"lux\"\r\n\r\n66.66\r\n--boundary\r\nContent-Disposition: form-data; name=\"visible\"\r\n\r\n777\r\n--boundary\r\nContent-Disposition: form-data; name=\"ir\"\r\n\r\n888\r\n--boundary\r\nContent-Disposition: form-data; name=\"full\"\r\n\r\n999\r\n--boundary--";
      int httpCode = http.POST(payload);
      Serial.print("httpCode: ");
      Serial.print(httpCode);
    }
  }

  delay(10000);
}