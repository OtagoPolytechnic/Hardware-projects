# Smart Plant Irrigation System
Plants are living organisms, each with specific needs that must be timely met. Many plant owners see them wither before they can flourish. 
Since getting plants installed in D-block, it'd make plant maintenance so much easier if it could be automated. The plants are currently watered randomly by anyone who remembers and are stored in the provided pots on dinner plates - not a very efficient setup.

As IT students, we're not gardeners nor do we have knowledge catered to the specific maintenance of Spider plants.
This is where the idea for an automatic smart home irrigation system came about. 
A simple system that keeps track of a plant’s moisture and humidity levels with automatic watering to allow plant owners to ensure their plants stay well maintained.

## Design Plan

<img width="800" height="840" alt="5-more-buttons" src="https://github.com/user-attachments/assets/b9bf6377-0ab0-4c08-98a4-dcc377755bca" />


The smart irrigation system (SIS) has a flexible scope, allowing for many different versions to be ideated. Each version was based on features we could implement and time/material constraints.

**Version 1:**
 - Measures the soil moisture for any given plant and, based on a threshold, determines if the plant needs watering
 - Photoresistor used to detect light level OR humidity sensor
 - Light turns on to let plant owner know that it needs watering/environment change
 - LCD screen displays moisture and environment, if it's dispensing water, ON/OFF, and plant name
 - Watchdog timer and FSM

**Version 2:**
 - A pump dispenses water based on the threshold
 - Buttons for user interation/changing info displayed on screen/on and off

**Version 3 (USB Serial communication and LoRa):**
 - App that sends and receives plant data
 - Arduinos sending/receiving plant info/commands based on plant info from plantapi
 - Dispensing water based on received command/info

**Version 4:**
 - 3D Printed arduino case that allows for clipping on and a pot

Version 1 is the first scope decided on for the main project idea. Version 2 implemented user interaction and automatic dispensing. This was the fully fledged market-ready version that we are aiming for.
Version 3 and 4 were more about the final product we want to produce. 

## Parts
|Part|Quantity|Link|
|---|---|---|
Arduino Nano		|     	x1 |https://www.jaycar.co.nz/arduino-compatible-nano-board/p/XC4414
Plastic for 3D print (?) | |
Soil moisture sensor	|	x1| https://www.jaycar.co.nz/duinotech-arduino-compatible-soil-moisture-sensor-module/p/XC4604|
12V Pump				|	x1| https://www.dicksmith.co.nz/dn/buy/zoestore-42w-mini-brushless-submersible-water-pump-for-fish-tank-aquarium-h15455/|
Water level sensor 		|	x1| https://epartners.co.nz/products/mp7006|
Vinyl tube				|	x1|https://www.bunnings.co.nz/pope-clear-vinyl-tubing-3mm-x-5m_p0235184 |
Transistor 				|	x1| Hardware room |
5k Ohm Resistor			|	x1| Hardware room |
1k Ohm Resistor			|	x1| Hardware room|
12V DC Power Adaptor	|	x1| https://www.dicksmith.co.nz/dn/buy/snap-deal-12v-1a-power-supply-ac-to-dc-adapter-7955281543409/|
DC Socket Adaptor	|		x1|https://www.jaycar.co.nz/21mm-dc-socket-with-screw-terminals/p/PA3713 |
30cm LED Strip			|	x1| Hardware room|
Buttons    |    x4| Hardware room|
LCD Screen | x1 |Hardware room|

## Resources
|Description|Link|
|---|---|
|Solenoid valve hookup vid | https://www.youtube.com/watch?v=ioSYlxHlYdI|
|Better hookup vid| https://techatronic.com/how-to-make-smart-irrigation-project/ <br/>https://arduinogetstarted.com/tutorials/arduino-automatic-irrigation-system|
|Plant API | https://perenual.com/docs/api|
|Basic Soil Moisture Sensor Project|https://projecthub.arduino.cc/Aswinth/soil-moisture-sensor-with-arduino-91c818|
|Soil Moisture Sensor w led and buzzer|https://projecthub.arduino.cc/nikolaiapalis/simple-soil-moisture-sensor-ec23c7|
|Light Sensor | https://jxxcarlson.medium.com/measuring-light-intensity-with-arduino-a575765c0862|
|Basic Irrigation Project | https://www.hackster.io/lakshyajhalani56/smart-irrigation-soil-moisture-sensor-arduino-water-pump-96030e|
|Irrigation w Pump | https://www.hackster.io/Techatronic/smart-irrigation-system-using-arduino-aaea36|
|Full System w 3D Design |https://www.instructables.com/Automatic-Smart-Plant-Pot-DIY-3D-Printed-Arduino-S/ <br/> https://www.thingiverse.com/thing:3537287|


			
