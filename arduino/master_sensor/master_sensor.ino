#include <SoftwareSerial.h> // Communitcate to ESP
#include "DHT.h" // DHT library
#include <OneWire.h> // Used for soil temp probe
#include <DallasTemperature.h> // Used for soil temp probe
#include <Wire.h> // TSL2591 light sensor
#include <Adafruit_Sensor.h> // TSL2591 light sensor
#include "Adafruit_TSL2591.h" // TSL2591  light sensor (Dynamic Range: 600M:1, Max Lux: 88k)

// Pins
#define MOISTURE_PIN 0 // Moisture sensor data connected to analog pin 0
#define DHTPIN 2
#define SOIL_TEMP_PIN 3 // Soil temp data pin
#define ESP_TX 11
#define ESP_RX 10

float moisture_raw = 0.0;
//float moisture = 0; //scale from 0-10 dry to wet

// DHT-22 Humidity/Air Temp Vars
#define DHTTYPE 22 
DHT dht(DHTPIN, DHTTYPE);
struct dht_data_struct
{
  float humidity = 0.0;
  float air_temp_c = 0.0;
  float air_temp_f = 0.0;
  float heat_index_c = 0.0;
  float heat_index_f = 0.0;
};
dht_data_struct dht_data; // holds all of dht22 data

struct light_sensor
{
  uint32_t lum = 0;
  uint16_t ir = 0;
  uint16_t full = 0;
  uint16_t visible = 0;
  float lux = 0;
};
light_sensor light_data; // holds all of the TSL2591 light sensor data

SoftwareSerial esp_serial(ESP_TX, ESP_RX); // RX, TX , Used to send data to ESP8266

OneWire oneWire(SOIL_TEMP_PIN);
DallasTemperature sensors(&oneWire);
float Celcius=0;
float Fahrenheit=0;

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

// Functions
float get_moisture();
void get_dht(dht_data_struct & dht_in);
void configureSensor(void); // tsl2591 lux
void displaySensorDetails(void); //lux 
void advancedRead(light_sensor & light_data); //lux


/////////////////////////////////////////////////

void setup() {
  // setup code, runs once:

  // Start serial for debugging
  Serial.begin(9600);
  while(!Serial); //wait until serial is setup

  esp_serial.begin(9600); // esp serial connection

  //DHT22
  Serial.println(F("DHT22 test!"));
  dht.begin();

  //Soil temp
  sensors.begin();

    Serial.println(F("Starting Adafruit TSL2591 Test!"));
  
  if (tsl.begin()) 
    Serial.println(F("Found a TSL2591 sensor"));
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
    
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Configure the sensor */
  configureSensor();

  Serial.println("Setup Finished.");
}

void loop() {
  // put your main code here, to run repeatedly:

  moisture_raw = get_moisture();
  Serial.print("Moisture: ");
  Serial.print(moisture_raw);

  get_dht(dht_data);
  Serial.print(F(" Humidity: "));
  Serial.print(dht_data.humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(dht_data.air_temp_c);
  Serial.print(F("°C "));
  Serial.print(dht_data.air_temp_f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(dht_data.heat_index_c);
  Serial.print(F("°C "));
  Serial.print(dht_data.heat_index_f);
  Serial.print(F("°F"));

  //Soil temp probe ***CHANGE TO USE STRUCT AND CALL FUNC****
  sensors.requestTemperatures();
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  //Serial.print(Celcius);
  Serial.print(" Soil temp F: ");
  Serial.print(Fahrenheit);  

  advancedRead(light_data);

  Serial.print(F(" IR: ")); Serial.print(light_data.ir);  Serial.print(F("  "));
  Serial.print(F("Full: ")); Serial.print(light_data.full); Serial.print(F("  "));
  Serial.print(F("Visible: ")); Serial.print(light_data.visible); Serial.print(F("  "));
  Serial.print(F("Lux: ")); Serial.println(light_data.lux, 6);

  delay(5000);
}
/////////////////////////////////////////////////

////////////FUNCTIONS///////////////////////////

float get_moisture()
{
  moisture_raw = analogRead(MOISTURE_PIN); //connect sensor to Analog 0
  //return ((DRY_SOIL - moisture_raw)/(DRY_SOIL - WATERED)) * 10.0;
  return moisture_raw;
}

void get_dht(dht_data_struct & dht_in)
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  dht_in.humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  dht_in.air_temp_c = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  dht_in.air_temp_f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(dht_in.humidity) || isnan(dht_in.air_temp_c) || isnan(dht_in.air_temp_f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  dht_in.heat_index_f = dht.computeHeatIndex(dht_in.air_temp_f, dht_data.humidity);
  // Compute heat index in Celsius (isFahreheit = false)
  dht_in.heat_index_c = dht.computeHeatIndex(dht_in.air_temp_c, dht_data.humidity, false);
  return;
}

void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  delay(500);
}

void advancedRead(light_sensor & light_data)
{
  light_data.lum = tsl.getFullLuminosity();
  light_data.ir = light_data.lum >> 16;
  light_data.full = light_data.lum & 0xFFFF;
  light_data.visible = light_data.full - light_data.ir;
  light_data.lux = tsl.calculateLux(light_data.full, light_data.ir);
}
