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
const char delimiter = ';';

String get_payload();
String get_data(const String & sensor_data, int & pos, const int & len);

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  // wait for WiFi connection

  WiFiClient client;
  HTTPClient http;

  if(http.begin(client, "http://" + String(IP) + ":8000/sensors/send_data/"))
  {
    http.addHeader("Content-Type", "multipart/form-data; boundary=boundary");

    //String payload = "--boundary\r\nContent-Disposition: form-data; name=\"sensor_id\"\r\n\r\n7\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_soil\"\r\n\r\n11.11\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_room\"\r\n\r\n22.22\r\n--boundary\r\nContent-Disposition: form-data; name=\"humidity\"\r\n\r\n33.33\r\n--boundary\r\nContent-Disposition: form-data; name=\"heat_index\"\r\n\r\n44.44\r\n--boundary\r\nContent-Disposition: form-data; name=\"moisture\"\r\n\r\n55.55\r\n--boundary\r\nContent-Disposition: form-data; name=\"lux\"\r\n\r\n66.66\r\n--boundary\r\nContent-Disposition: form-data; name=\"visible\"\r\n\r\n777\r\n--boundary\r\nContent-Disposition: form-data; name=\"ir\"\r\n\r\n888\r\n--boundary\r\nContent-Disposition: form-data; name=\"full\"\r\n\r\n999\r\n--boundary--"; // Get formatted HTTP POST request from arduino via serial String payload = get_payload();
    String payload = get_payload();

    // Send post request with payload (upload sensor data to Django web server)
    int httpCode = http.POST(payload);
  }

  delay(100);
}

String get_payload()
{
  String sensor_data = "";
  int pos = 0;
  int len = 0;
  String data = "";
  String payload = "";

  recieve_data(sensor_data);
  // sensor_data = "9;71.15;73.76;59.50;73.63;621.00;243.84;5615;1419;7034;";
  len = sensor_data.length();

  // Extract data from sensor_data and format HTTP POST form request
  payload = "--boundary\r\nContent-Disposition: form-data; name=\"sensor_id\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_soil\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"temp_room\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;
  
  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"humidity\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"heat_index\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"moisture\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"lux\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"visible\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"ir\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary\r\nContent-Disposition: form-data; name=\"full\"\r\n\r\n";
  data = get_data(sensor_data, pos, len);
  payload += data;

  payload += "\r\n--boundary--";
  return payload;
}

String get_data(const String & sensor_data, int & pos, const int & len)
{
  String data = "";

  if(pos >= len)
  {
    // out of bounds!
    data = "-";
    return data;
  }

  while(pos < len && sensor_data[pos] != delimiter) 
  {
    data += sensor_data[pos];
    ++pos;
  }
  ++pos;
  return data;
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
    recieve_data(data);
    
  return 0;
}

/*
// If I need to send data to Arduino in the future...

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