<h1 align="center" font-weight="bold">Wireless Plant Monitoring</h1>

### About
**Goal:** To host a website that tracks plant data through wireless sensors.

I'm developing a system of Arduinos with sensors to monitor plants connected over RF to a master module. The master module has an ESP8266 WiFi chip that uploads data to a Django web server via HTTP POST request.

### [Click here to check out the website!](http://matthewhubbard.pythonanywhere.com "Plant Monitor Website")

![Demo](https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/Pictures/Demo2.gif)

<h2 align="center">System Diagram</h2>

<p align="center">
<img align="center" src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/Flow_Diagram.png" height="75%" width="75%">
</p>

<h2 align="center">Technology Stack</h2>
<hr>

<p align="center">
<img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/C++_logo.svg" height="8%" width="8%"><img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/Arduino_logo.svg" height="13%" width="13%">
</p>

<p align="center">
<img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/html_5_logo.svg" height="9%" width="9%"><img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/css_3_logo.svg" height="9%" width="9%"><img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/javascript_logo.png" height="13%" width="13%"><img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/Jquery_logo.png" height="12%" width="12%"><img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/Bootstrap_logo.svg" height="11%" width="11%"><img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/Chartjs_logo.png" height="10%" width="10%">
</p>

<p align="center">
<img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/python_logo.svg" height="22%" width="22%"><img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/SQLite_logo.png" height="22%" width="22%"><img src= "https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/stack_logos/Django_logo.svg" height="22%" width="22%">
</p>

### Master Sensor With Wifi
![Master Sensor With Wifi](https://github.com/Matthew-Hubbard/Plant_Monitor_Project/blob/master/plant_back_end/sensors/static/images/master_prototype_1.jpg)

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


