#include <SoftwareSerial.h> // Communitcate to ESP
//#include "DHT.h" // DHT library
//#include <OneWire.h> // Used for soil temp probe
//#include <DallasTemperature.h> // Used for soil temp probe
//#include <Wire.h> // TSL2591 light sensor
//#include <Adafruit_Sensor.h> // TSL2591 light sensor
//#include "Adafruit_TSL2591.h" // TSL2591  light sensor (Dynamic Range: 600M:1, Max Lux: 88k)
#include <RF24Network.h> // nrf2401+
#include <RF24.h> // nrf2401+
#include <SPI.h> // nrf2401+

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);

// nrf2401+
RF24Network network(radio);
#define NUM_NODES 2 // including master sensor node
//#define NUM_NODES 4 // including master sensor node
//#define MAX_TRIES 4 // max number of tries to request data from sensor before going to the next one
#define MAX_TRIES 10 // max number of tries to request data from sensor before going to the next one
#define MAX_AVAIL_TRIES 30000
#define MAX_AVAIL_TRIES 20000
//const uint16_t node_array[NUM_NODES] = {00, 01, 02, 03};
const uint16_t node_array[NUM_NODES] = {00, 01};
struct packet
{
  uint8_t sensor_id;
  uint8_t packet_number;
  char data[30];
};

// Pins
// #define MOISTURE_PIN 0 // Moisture sensor data connected to analog pin 0
// #define DHTPIN 2
// #define SOIL_TEMP_PIN 3 // Soil temp data pin
#define ESP_TX 4
#define ESP_RX 5

// float moisture_raw = 0.0;

// // DHT-22 Humidity/Air Temp Vars
// #define DHTTYPE 22 
// DHT dht(DHTPIN, DHTTYPE);
// struct dht_data_struct
// {
//   float humidity = 0.0;
//   float air_temp_c = 0.0;
//   float air_temp_f = 0.0;
//   float heat_index_c = 0.0;
//   float heat_index_f = 0.0;
// };
// dht_data_struct dht_data; // holds all of dht22 data

// struct light_sensor
// {
//   uint32_t lum = 0;
//   uint16_t ir = 0;
//   uint16_t full = 0;
//   uint16_t visible = 0;
//   float lux = 0;
// };
// light_sensor light_data; // holds all of the TSL2591 light sensor data

SoftwareSerial esp_serial(ESP_TX, ESP_RX); // RX, TX , Used to send data to ESP8266
const int SERIAL_DELAY = 250;
const int SERIAL_ITTER = 100;
String msg = "";

// OneWire oneWire(SOIL_TEMP_PIN);
// DallasTemperature sensors(&oneWire);
// float Celcius=0;
// float temp_soil=0;

// Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

// int sensor_id = 0;
// String payload = "";

// // Functions
// float get_moisture();
// void get_dht(dht_data_struct & dht_in);
// void configureSensor(void); // tsl2591 lux
// void displaySensorDetails(void); //lux 
// void advancedRead(light_sensor & light_data); //lux
int recieve_data(String & data, SoftwareSerial & serial_in);
int send_data(const String & data, SoftwareSerial & espSerial);
void gather_data(String & payload1, String & payload2);

//////////////////////////////////////////////////////////////

void setup() {
  // Start serial for debugging
  Serial.begin(9600);
  // while(!Serial); //wait until serial is setup

  esp_serial.begin(9600); // esp serial connection

  SPI.begin();
  radio.begin();
  network.begin(90, node_array[0]);
  Serial.println("[Arduino] : nrf2401+ setup complete.");

  // //DHT22
  // Serial.println(F("[Arduino] : DHT22 test!"));
  // dht.begin();

  // //Soil temp
  // sensors.begin();

  // Serial.println(F("[Arduino] : Starting Adafruit TSL2591 Test!"));
  
  // if (tsl.begin()) 
  //   Serial.println(F("[Arduino] : Found a TSL2591 sensor"));
  // else 
  // {
  //   Serial.println(F("[Arduino] : No sensor found ... check your wiring?"));
  //   while (1);
  // }
    
  // /* Display some basic information on this sensor */
  // displaySensorDetails();
  
  // /* Configure the sensor */
  // configureSensor();

  Serial.println("[Arduino] : Waiting for ESP to finish setup.");
  delay(4000); // wait for ESP
  Serial.println("----------------------------------------------------------------");
}

//#define DEBUG_RF

void loop() {
  for (int cur_id = 1; cur_id < NUM_NODES; ++cur_id)
  {
    bool responded = false;
    network.update();
    bool both_packets = false;
    String payload = "";
    int cur_packet = 1;
    int count = 0;

    while (!both_packets && count < MAX_TRIES)
    {
      ++count;
#ifdef DEBUG_RF
      Serial.print("*");
#endif

      network.update();

      packet request_packet;
      request_packet.sensor_id = cur_id;
      request_packet.packet_number = cur_packet;
      strcpy(request_packet.data, "request");

//#ifdef DEBUG_RF
      Serial.print("[Arduino] : Requesting Sensor ");
      Serial.print(cur_id);
      Serial.print(" packet ");
      Serial.println(cur_packet);
//#endif

      radio.flush_tx();  // MAYBE REMOVE???
      
      RF24NetworkHeader header_out(node_array[cur_id]);         // (Adress where the data is sent)
      /*bool ok = */ network.write(header_out, & request_packet, 32); // Sending request to sensor

      //delay(100); // Wait for sensor to gather data

      // TODO update to timer
      int avail_count = 0;
      while(!network.available() && avail_count < MAX_AVAIL_TRIES) { network.update(); ++avail_count;}

      //network.update();
      while (network.available() && !both_packets)
      {
        radio.flush_rx(); // MAYBE REMOVE???

        //Serial.print("-");
        RF24NetworkHeader header_in;
        packet sensor_data;
        network.read(header_in, &sensor_data, 32);


//#ifdef DEBUG_RF
        Serial.print("[Sensor ");
        Serial.print(sensor_data.sensor_id);
        Serial.print("] : ");
        Serial.println(sensor_data.data);
//#endif
        if (sensor_data.sensor_id = cur_id && sensor_data.packet_number == cur_packet)
        {
          payload += String(sensor_data.data);
          if (sensor_data.packet_number == 2)
          {
#ifdef DEBUG_RF
            Serial.println("[Arduino] : Got both packets!");
#endif
            both_packets = true;
          }
          ++cur_packet;
        }
        else
        {
#ifdef DEBUG_RF
          Serial.println("[Arduino][ERROR] : Packet didn't match!");
          Serial.print("[Arduino] : both_packets: ");
          Serial.println(both_packets);
#endif
        }
      }
    }
    Serial.print("[Arduino] [Sensor ");
    Serial.print(cur_id);
    Serial.print(" PAYLOAD] : ");
    Serial.println(payload);
    if(payload != "")
    {
      Serial.println("[Arduino] : Sending payload to ESP for HTTP upload.");
      send_data(payload, esp_serial);
      delay(2000);
    }
  }

  Serial.println("\n[Arduino] : Sent all data to ESP!");
  /*
  Serial.print("[Arduino] : Sleeping");
  for (int i = 0; i < 10; ++i)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  */


//  gather_data(payload1, payload2);
//  String payload = payload1 + payload2;

//  send_data(payload, esp_serial);

  //Serial.print("\npayload: "); Serial.println(payload);
//  Serial.println("\n[Arduino] : Sent all data to ESP!");

  //Serial.println("[Arduino] : Waiting for ESP to make HTTP Post request...\n");
  //sensor_id = (sensor_id + 1) % 4;
}

/////////////////////-FUNCTIONS-///////////////////////////

// void gather_data(String & payload1, String & payload2)
// {
//   Serial.print("[Arduino] : Sensor ID: "); Serial.print(sensor_id); Serial.print(" ");
//   moisture_raw = get_moisture();
//   Serial.print("Moisture: ");
//   Serial.print(moisture_raw);

//   get_dht(dht_data);
//   Serial.print(F(" Humidity: "));
//   Serial.print(dht_data.humidity);
//   Serial.print(F("%  Temperature: "));
//   Serial.print(dht_data.air_temp_c);
//   Serial.print(F("°C "));
//   Serial.print(dht_data.air_temp_f);
//   Serial.print(F("°F  Heat index: "));
//   Serial.print(dht_data.heat_index_c);
//   Serial.print(F("°C "));
//   Serial.print(dht_data.heat_index_f);
//   Serial.print(F("°F"));

//   //Soil temp probe ***CHANGE TO USE STRUCT AND CALL FUNC****
//   sensors.requestTemperatures();
//   Celcius=sensors.getTempCByIndex(0);
//   temp_soil=sensors.toFahrenheit(Celcius);
//   //Serial.print(Celcius);
//   Serial.print(" Soil temp F: ");
//   Serial.print(temp_soil);  

//   advancedRead(light_data);

//   Serial.print(F(" IR: ")); Serial.print(light_data.ir);  Serial.print(F("  "));
//   Serial.print(F("Full: ")); Serial.print(light_data.full); Serial.print(F("  "));
//   Serial.print(F("Visible: ")); Serial.print(light_data.visible); Serial.print(F("  "));
//   Serial.print(F("Lux: ")); Serial.println(light_data.lux, 6);

//   //Send to ESP
//   payload1 = String(sensor_id) + String(';') + \
//   String(temp_soil) + String(';') + \
//   String(dht_data.air_temp_f) + String(';') + \
//   String(dht_data.humidity) + String(';') + \
//   String(dht_data.heat_index_f) + String(';') + \
//   String(moisture_raw) + String(';');

//   payload2 = String(light_data.lux) + String(';') + \
//   String(light_data.visible) + String(';') + \
//   String(light_data.ir) + String(';') + \
//   String(light_data.full) + String(';');

//   return;
// }

// float get_moisture()
// {
//   moisture_raw = analogRead(MOISTURE_PIN); //connect sensor to Analog 0
//   //return ((DRY_SOIL - moisture_raw)/(DRY_SOIL - WATERED)) * 10.0;
//   return moisture_raw;
// }

// void get_dht(dht_data_struct & dht_in)
// {
//   // Reading temperature or humidity takes about 250 milliseconds!
//   // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//   dht_in.humidity = dht.readHumidity();
//   // Read temperature as Celsius (the default)
//   dht_in.air_temp_c = dht.readTemperature();
//   // Read temperature as Fahrenheit (isFahrenheit = true)
//   dht_in.air_temp_f = dht.readTemperature(true);

//   // Check if any reads failed and exit early (to try again).
//   if (isnan(dht_in.humidity) || isnan(dht_in.air_temp_c) || isnan(dht_in.air_temp_f)) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }

//   // Compute heat index in Fahrenheit (the default)
//   dht_in.heat_index_f = dht.computeHeatIndex(dht_in.air_temp_f, dht_data.humidity);
//   // Compute heat index in Celsius (isFahreheit = false)
//   dht_in.heat_index_c = dht.computeHeatIndex(dht_in.air_temp_c, dht_data.humidity, false);
//   return;
// }

// void configureSensor(void)
// {
//   // You can change the gain on the fly, to adapt to brighter/dimmer light situations
//   //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
//   tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
//   //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
//   // Changing the integration time gives you a longer time over which to sense light
//   // longer timelines are slower, but are good in very low light situtations!
//   //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
//   // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
//   tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
//   // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
//   // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
//   // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

//   /* Display the gain and integration time for reference sake */  
// #ifdef DEBUG_TSL
//   Serial.println(F("------------------------------------"));
//   Serial.print  (F("Gain:         "));
//   tsl2591Gain_t gain = tsl.getGain();
//   switch(gain)
//   {
//     case TSL2591_GAIN_LOW:
//       Serial.println(F("1x (Low)"));
//       break;
//     case TSL2591_GAIN_MED:
//       Serial.println(F("25x (Medium)"));
//       break;
//     case TSL2591_GAIN_HIGH:
//       Serial.println(F("428x (High)"));
//       break;
//     case TSL2591_GAIN_MAX:
//       Serial.println(F("9876x (Max)"));
//       break;
//   }
//   Serial.print  (F("Timing:       "));
//   Serial.print((tsl.getTiming() + 1) * 100, DEC); 
//   Serial.println(F(" ms"));
//   Serial.println(F("------------------------------------"));
//   Serial.println(F(""));
// #endif
// }

// void displaySensorDetails(void)
// {
// #ifdef DEBUG_TSL
//   sensor_t sensor;
//   tsl.getSensor(&sensor);
//   Serial.println(F("------------------------------------"));
//   Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
//   Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
//   Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
//   Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
//   Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
//   Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
//   Serial.println(F("------------------------------------"));
//   Serial.println(F(""));
//   delay(500);
// #endif
// }

// void advancedRead(light_sensor & light_data)
// {
//   light_data.lum = tsl.getFullLuminosity();
//   light_data.ir = light_data.lum >> 16;
//   light_data.full = light_data.lum & 0xFFFF;
//   light_data.visible = light_data.full - light_data.ir;
//   light_data.lux = tsl.calculateLux(light_data.full, light_data.ir);
// }

// Send data via software serial
int send_data(const String & data, SoftwareSerial & serial_out)
{
  // if(data == "")
  //   return -1;

  String recieved = ""; // message back from serial
  int i = 0;
  serial_out.flush();
  
  //Debugging info
  Serial.println("[Arduino] : Writing data over serial to ESP...");
  Serial.println("[Arduino] : " + data + " -> [ESP]");
  
  // Sending data over serial
  serial_out.write(data.c_str());

  delay(SERIAL_DELAY);

  // Now wait for confirmation from ESP. ESP Should send data back to confirm.
  while(!serial_out.available() && i < SERIAL_ITTER) {++i;} // Wait for serial_out read buffer to be empty...

  if(i == SERIAL_ITTER - 1 )
  {
    Serial.print("[Arduino] [ERROR] : serial_out not available to read! (read buffer not empty!)");
    return -1;
  }

  i = 0;
  while(recieved != data && i < SERIAL_ITTER)
  {
    recieved = serial_out.readString();
    ++i;
  }
  if(i >= SERIAL_ITTER - 1)
  {
    Serial.println("[Arduino] [ERROR] : Couldn't recieve confirmation from ESP. (recieved: " + recieved + ")");
    return -2;
  }
  else
  {
    Serial.println("[ESP] : " + recieved);
    if(recieved == data)
      Serial.println("[Arduino] : Transmition success!");
    else
    {
        Serial.println("[Arduino] : Transmition failed.");
    }
    
    serial_out.flush();
    return 0;
  }
}

/*
//recieve data from ESP
int recieve_data(String & data, SoftwareSerial & serial_in)
{
  data = "";
  int i = 0;

  // Wait for serial_in read buffer to be empty...
  while(!serial_in.available() && i < SERIAL_ITTER) {++i;} 

  if(i == SERIAL_ITTER - 1 )
  {
    // serial_in not available to read! (read buffer not empty!)");
    recieve_data(data, serial_in);
    return -1;
  }

  //get data from Arduino Sensor
  i = 0;
  while (serial_in.available() && i < SERIAL_ITTER)
  //while (serial_in.available())
  {
    data = serial_in.readString();
    ++i;
  }

  Serial.println("[ESP] : " + data);

  if(i >= SERIAL_ITTER - 1)
  {
    //[ERROR] : Couldn't recieve data
    recieve_data(data, serial_in);
    return -2;
  }

 // Send data back to ESP to confirm transmition

// int num_bytes = serial_in.availableForWrite(); // check if we have anything still in write buffer
// if(num_bytes > 0)
// {
   // [ERROR] : bytes already in write buffer...
   serial_in.flush();
 //}
 
 // Sending data over serial to arduino sensor
 delay(SERIAL_DELAY);
 Serial.println("[ARDUINO] : " + data + " -> [ESP]");
 if(data != "")
   serial_in.write(data.c_str());
  else
  {
    recieve_data(data, serial_in);
  }
  
 return 0;
}
*/
