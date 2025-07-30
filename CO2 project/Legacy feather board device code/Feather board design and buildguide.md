### TAKEN FROM WEATHER STATION REPO MAY

We were tasked with assisting Martin's CO2 project with new sensors, including testing/verifying existing ones and ensuring any new ones are giving correct readings.

Martin has requested as many CO2 sensors as we can provide. 

### Existing hardware
We tested the Adafruit Feather 32u4's and found that they can only handle one sensor per board. This means they are ideal for the CO2 sensors. 

### Results
The feathers only support the use of tiny lora which means we can only use abp on them we have figured out how to get this working.
We know have data uploading to the ttn using this format.
We have also decided against buying more feathers and swapping to ttgos they are slightly cheaper and more grunty so future sensors that are made once all feathers are used up will use ttgos

## How to build and solder a co2 sensor

This guide will walk you through the steps to build a CO2 sensor using a copper  breadboard and a CO2 sensor module. The images provided in this wiki demonstrate the key steps in assembly and soldering.

### Materials needed:

* Copper breadboard
* CO2 sensor module
* Header pins
* Microcontroller board (e.g., Adafruit Feather)
* Jumper wires
* Soldering iron and solder
* Wire cutters
* Multimeter (optional, for testing)
* Dremel with mini saw blade

### Step 1: Prepare the Copper Board
### Cutting and Marking
Cut a 91mm x 44mm copper board and score it down the middle separating the copper strips. Using a dremel with a mini saw blade is the easiest. Use a multimeter to ensure you have scored it correctly.  
![board_scored](https://github.com/user-attachments/assets/77d77929-43fb-43e9-b490-d967b83074ea)  

### Step 2: Add header pins
Use bluetack to holder the header pins in place in the same layout as below. This layout ensure enough room for the sensor on the board. Using pins like this ensures that wires can be swapped out easily as decaying jumper wires has been the main cause of failure in the past.  
![header_pins](https://github.com/user-attachments/assets/d6acb11a-8003-436d-be08-c5e2781509e4)  

Solder the header pins onto the breadboard as shown.  
![board_soldered](https://github.com/user-attachments/assets/e14c0036-792b-4ed4-8400-364943e1b428)  

### Solder the co2 Sensor  
Use bluetack to hold this in if there isn't a clamp available. just be mindful that the bluetach can become quite sticky and runny so don't have it covering any of the holes. Using a larger blob of bluetack helps for getting any that is left remaining on the sensors.


![co2_sensor_top](https://github.com/user-attachments/assets/10274672-cd76-41ee-ad4a-566577b6ca64)


![co2_sensor_under](https://github.com/user-attachments/assets/80332eef-3a02-40c0-8b49-3fc6aa8093e9)  

# Code for CO2
The code for a CO2 sensor is found [here.](https://github.com/OtagoPolytechnic/Cloudy-with-a-Chance-of-LoRa/tree/staging/sensors/co2_sensor_32u4)

Using the Arduino IDE ensure the TinyLora Library is loaded via Sketch > Add .ZIP library\
![image](https://github.com/user-attachments/assets/c042e491-8475-4b37-8948-b377a839c095)

The board is an Adafruit Feather 32u4.

Register a new device on [The Things Network Sandbox](https://au1.cloud.thethings.network/console)

![image](https://github.com/user-attachments/assets/b500c6ad-0d62-4061-bbb9-632db5f31e08)

![image](https://github.com/user-attachments/assets/23e3d278-a663-4f1f-b118-87a5b81ba7c5)

Ensure that Network Settings > Advanced MAC settings frame counter is ticked.

![image](https://github.com/user-attachments/assets/8073055e-1207-4e26-ac37-9295a7d5562b)


Custom Javascript payload formatter:
```
function Decoder(bytes, port) {
  if (bytes.length !== 4) {
    return null; // Invalid payload length
  }

  let decoded = {};

  // Combine bytes into 16-bit integers
  let tempRaw = (bytes[1] << 8) | bytes[0]; // Temperature is in the first two bytes
  let co2Raw = (bytes[3] << 8) | bytes[2];  // CO2 concentration is in the last two bytes

  // Decode temperature and CO2
  // Temperature is scaled by 100 (fixed-point with 2 decimal places)
  let temperature = tempRaw / 100.0;

  // CO2 concentration is directly used
  let CO2 = co2Raw;

  decoded.temperature = temperature;
  decoded.CO2 = CO2;

  return decoded;
}
```

