#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID

//#define STASSID "Samsung Galaxy S7 8574"
//#define STAPSK  "auhq4212"
//#define IP "192.168.43.225"

#define STASSID "CenturyLink0853"
#define STAPSK  "htizeeynhc9wm7"
#define IP "192.168.0.72"

#endif

const char* ssid = STASSID;
const char* password = STAPSK;

int recieve_data(String & data);
int send_data(const String & data);
const int SERIAL_DELAY = 500;
const int SERIAL_ITTER = 500;

void setup() {

  // Serial.begin(115200);
  // Serial.setDebugOutput(true);

  //Serial.begin(115200);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
  
  //send that we're ready and wait for Arduino to be ready.
  /*
  String msg = "";
  while(msg != "ARDUINO READY")
  {
    send_data("ESP READY");
    delay(1000);
    recieve_data(msg);
  }
  */
}

void loop() {
  // wait for WiFi connection

  String sensor_data = "";
  
  WiFiClient client;
  HTTPClient http;
  //Serial.print("[HTTP] begin...\n");
  //Serial.print("Connecting to " + String(IP) + " with http...\n");
  if(http.begin(client, "http://" + String(IP) + ":8000/sensors/send_data/"))
  {
    http.addHeader("Content-Type", "multipart/form-data; boundary=boundary");
    
    //String payload = "--boundary\r\nContent-Disposition: form-data; name=\"sensor_id\"\r\n\r\n7\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_soil\"\r\n\r\n11.11\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_room\"\r\n\r\n22.22\r\n--boundary\r\nContent-Disposition: form-data; name=\"humidity\"\r\n\r\n33.33\r\n--boundary\r\nContent-Disposition: form-data; name=\"heat_index\"\r\n\r\n44.44\r\n--boundary\r\nContent-Disposition: form-data; name=\"moisture\"\r\n\r\n55.55\r\n--boundary\r\nContent-Disposition: form-data; name=\"lux\"\r\n\r\n66.66\r\n--boundary\r\nContent-Disposition: form-data; name=\"visible\"\r\n\r\n777\r\n--boundary\r\nContent-Disposition: form-data; name=\"ir\"\r\n\r\n888\r\n--boundary\r\nContent-Disposition: form-data; name=\"full\"\r\n\r\n999\r\n--boundary--";

    // Get sensor data from serial connection
    recieve_data(sensor_data);
    String payload = "--boundary\r\nContent-Disposition: form-data; name=\"sensor_id\"\r\n\r\n";
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_soil\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_room\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"humidity\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"heat_index\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"moisture\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"lux\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"visible\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"ir\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"full\"\r\n\r\n";
    recieve_data(sensor_data);
    payload += sensor_data;
    payload += "\r\n--boundary--";

    //Serial.print("[HTTP] POST...\n");
    int httpCode = http.POST(payload);
    //Serial.print("httpCode: "); Serial.print(httpCode);
  }

  delay(10000);
}

int recieve_data(String & data)
{
  data = "";
  int i = 0;

  // Wait for Serial read buffer to be empty...
  while(!Serial.available() && i < SERIAL_ITTER) {++i;} 

  if(i == SERIAL_ITTER - 1 )
  {
    // Serial not available to read! (read buffer not empty!)");
    recieve_data(data);
    return -1;
  }

  //get data from Arduino Sensor
  i = 0;
  while (Serial.available() && i < SERIAL_ITTER)
  //while (Serial.available())
  {
    data = Serial.readString();
    ++i;
  }

  if(i >= SERIAL_ITTER - 1)
  {
    //[ERROR] : Couldn't recieve data
    recieve_data(data);
    return -2;
  }

 // Send data back to Arduino Sensor to confirm transmition

 int num_bytes = Serial.availableForWrite(); // check if we have anything still in write buffer
 if(num_bytes > 0)
 {
   // [ERROR] : bytes already in write buffer...
   Serial.flush();
 }
 
 // Sending data over serial to arduino sensor
 delay(SERIAL_DELAY);
 if(data != "")
   Serial.write(data.c_str());
  else
  {
    recieve_data(data);
  }
  
 return 0;
}

/*
int send_data(const String & data)
{
  String recieved = ""; // message back from serial
  int i = 0;
  int num_bytes = Serial.availableForWrite(); // check if we have anything still in write buffer
  if(num_bytes > 0)
  {
    Serial.flush();
  }
  
  // Sending data over serial
  Serial.write(data.c_str());

  delay(SERIAL_DELAY);

  // Now wait for confirmation from ESP. ESP Should send data back to confirm.

  while(!Serial.available() && i < SERIAL_ITTER) {++i;} // Wait for Serial read buffer to be empty...

  if(i == SERIAL_ITTER - 1 )
  {
    //Serial.print("[ESP] [ERROR] : Serial not available to read! (read buffer not empty!)");
    return -1;
  }

  i = 0;
  while(recieved != data && i < SERIAL_ITTER)
  {
    //while (Serial.available())
    {
      recieved = Serial.readString();
    }
    ++i;
  }
  if(i >= SERIAL_ITTER - 1)
  {
    return -2;
  }
  else
  {
    Serial.flush();
    return 0;
  }
}
*/