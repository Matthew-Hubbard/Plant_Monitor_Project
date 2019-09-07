# Wireless Plant Monitoring

### Demo
![Demo](https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/Pictures/Demo1.gif)

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
- [Arduino Pro Mini 3.3v](https://www.sparkfun.com/products/11114)
- [nrf24l01+ RF Transceiver](https://www.sparkfun.com/datasheets/Components/SMD/nRF24L01Pluss_Preliminary_Product_Specification_v1_0.pdf) [(guide)](https://medium.com/@benjamindavidfraser/arduino-nrf24l01-communications-947e1acb33fb)
- [DHT22 Humidity and Temperature Sensor](https://www.adafruit.com/product/385)
- [Capacitive Soil Moisture Sensor v1.2](https://www.dfrobot.com/product-1385.html)
- [TSL2591 Lux Sensor](https://learn.adafruit.com/adafruit-tsl2591)
- [DS18B20 Waterproof Temperature Sensor (Cable)](https://www.adafruit.com/product/381)


