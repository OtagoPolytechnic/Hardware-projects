# Smart Plant Irrigation System
## Purpose
Plants are living organisms, each with specific needs that must be timely met. Many plant owners see them wither before they can flourish. 
Since getting plants installed in D-block, it'd make plant maintenance so much easier if it could be automated. The plants are currently watered randomly by anyone who remembers and are stored in the provided pots on dinner plates - not a very efficient setup.

As IT students, we're not gardeners nor do we have knowledge catered to the specific maintenance of Spider plants.
This is where the idea for an automatic smart home irrigation system came about. 
A simple system that keeps track of a plant’s moisture and humidity levels with automatic watering to allow plant owners to ensure their plants stay well maintained.

## Instructions

A lot of the files in this directory are for informative purposes only. We currently do not have any code for the system. We created the physical system as seen in the version 1 wiring diagram below with some bare minimum code that turns the relay on and off for demo/testing purposes. 

This started as a solo project for embedded systems, so the files in this directory are from that. We have not implemented a full, working system yet. Use the code and information in the files provided to inform the implementation of the system as outlined in the following sections. I recommend starting with the links in the resources section at the bottom of this page.

To start off, create code that turns the relay on and off (refer to the last link in the resources section for assistance). **DO NOT** mess with the wiring of the relay/pump as they are fragile and should work as is. **ENSURE THE WATER PUMP IS FULLY SUBMERSED IN WATER WHEN TURNING IT ON!!! IT WILL BREAK IF NOT IN WATER.**
Once you have the relay system working, you can move on to the soil moisture sensor. The idea for this system is for the sensor to detect when the plant needs watering. When it does, the relay should turn on and water the plant until the moisture sensor detects that it's well watered (play around with the threshold of this). The LCD and LED are to display in a user-friendly way the status of the plant, so it's entirely optional.

## Design Plan
**Version 1 Wiring Diagram - Current ver**

<img width="800" height="600" src="https://github.com/user-attachments/assets/5e79ad9d-3a42-49b4-b5c9-1449e0347196" />

<hr />

**Version 2 Wiring Diagram**

<img width="800" height="840" alt="5-more-buttons" src="https://github.com/user-attachments/assets/b9bf6377-0ab0-4c08-98a4-dcc377755bca" />


<hr />

The smart irrigation system (SIS) has a flexible scope, allowing for many different versions to be ideated. Each version was based on features we could implement and time/material constraints.

**Version 1:**
 - Measures the soil moisture for any given plant and, based on a threshold, determines if the plant needs watering
 - Automatically dispenses water when needed

**Version 2:**
 - A pump dispenses water based on the threshold
 - LCD screen displays moisture and environment, if it's dispensing water, ON/OFF, and plant name
 - Photoresistor used to detect light level OR humidity sensor
 - Light turns on to let plant owner know that it needs watering/environment change
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
Arduino		|     	x1 |
Soil moisture sensor	|	x1| https://www.jaycar.co.nz/duinotech-arduino-compatible-soil-moisture-sensor-module/p/XC4604|
12V Pump				|	x1| https://www.dicksmith.co.nz/dn/buy/zoestore-42w-mini-brushless-submersible-water-pump-for-fish-tank-aquarium-h15455/|
Vinyl tube				|	x1|https://www.bunnings.co.nz/pope-clear-vinyl-tubing-3mm-x-5m_p0235184 |
12V AC Power Transformer	|	x1| https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.kerry-electronics.com%2FTransformer-transformer-30VA-%2F-30W-12V-AC-%2F-AC-kez0115-ket30&psig=AOvVaw3k_n2wBgM0b2tJKfDkyUYw&ust=1760662753120000&source=images&cd=vfe&opi=89978449&ved=0CBgQjhxqFwoTCMDZoYzCp5ADFQAAAAAdAAAAABAM|
30cm LED Strip			|	x1| Hardware room|
LCD Screen | x1 |Hardware room|
Relay | x1||

## Resources
|Description|Link|
|---|---|
|Basic Soil Moisture Sensor Project|https://projecthub.arduino.cc/Aswinth/soil-moisture-sensor-with-arduino-91c818|
|Soil Moisture Sensor w led and buzzer|https://projecthub.arduino.cc/nikolaiapalis/simple-soil-moisture-sensor-ec23c7|
|Irrigation w Pump - basic code and wiring diagram for connecting/testing pump + relay | https://www.hackster.io/Techatronic/smart-irrigation-system-using-arduino-aaea36|


			
