# Wireless Plant Monitoring
### About
**End goal:** To host a website that will track plant data through wireless sensors and tell you when they need watering.

I'm developing a system of Arduinos with sensors to monitor plants connected over RF that upload data to a full stack website.

Each sensor module will transmit data to the master module via 2.4Ghz RF. The master module will then send the data via serial into an ESP8266 Wifi module. The ESP8266 will then transmit the data as a JSON via HTTP to a Django web server. From there the data will be presented on a website using a Vue.js frontend.

#### Data Collected From each Wireless Sensor Module
- Air Temperature
- Soil Temperature
- Sunlight Luminosity (Lux)
- Humidity
- Soil Moisture

## Wiring Diagrams
### Sensor Module
![Fritzing Breadboard Wiring Diagram](https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/schematics/Plant_Monitoring_Project_v1.0_bb.png)
<sup>\*Fritzing files available in the 'schematics' directory.</sup>
#### Components
- Arduino Pro Mini 3.3v
- nrf24l01+ RF Transceiver
- DHT22 Humidity and Temperature Sensor
- Capacitive Soil Moisture Sensor v1.2
- [TSL2591 Lux Sensor](https://learn.adafruit.com/adafruit-tsl2591)
- DS18B20 Waterproof Temperature Sensor (Cable)


