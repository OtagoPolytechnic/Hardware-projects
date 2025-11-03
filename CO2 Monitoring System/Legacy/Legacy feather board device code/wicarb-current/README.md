
# Welcome to WiCarb!

  

This repository is filled with everything you should need to know about the new Carbon Dioxide monitoring project for Otago Polytechnic.

## Database

  

The database for sensor data requires two tables set up with the following fields;

#### ppm

|Name|Type|Null|Default|Extra|
|--|--|--|--|--|
|pID|int|No||AUTO_INCREMENT|
|pTime|timestamp|No|CURRENT_TIMESTAMP||
|ppm|int|No|||
|temp|int|No|||
|devID|varchar(20)|No|||


#### devices

|Name|Type|Null|Default|Extra|
|--|--|--|--|--|
|dnum|int|No||AUTO_INCREMENT|
|devID|varchar(20)|No||
|name|varchar(20)|No|||


## Website

 

The website doesn't have any other dependencies other than being able to run PHP, and to have direct access to the SQL database.

The `sql.inc.php` file must be updated with the database username and password. No other changes need to be made. 

  

## Sketches

  

The device sketches in this repo have some requirements before they can be flashed to a board. You'll need to download some libraries and board profiles in order to compile these sketches.

  

### Prerequisites

#### ESP8266

-  [Arduino IDE (1.8.X or higher)](https://www.arduino.cc/en/Main/Software)

-  [ESP8266 Core for Arduino IDE](https://github.com/esp8266/Arduino)

-  [MH-Z19 CO2 Sensor Library](https://github.com/crisap94/MHZ19)

-  [WiFi Manager](https://github.com/tzapu/WiFiManager)

#### LoRa Radio Node v1.0

-  [Arduino IDE (1.8.X or higher)](https://www.arduino.cc/en/Main/Software)

-  [MH-Z19 CO2 Sensor Library](https://github.com/crisap94/MHZ19)

- [TinyLoRa](https://github.com/adafruit/TinyLoRa)
