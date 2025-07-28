Add links and info for hardware

# LORA Radio Node V1

## More Info
For more information view https://werner.rothschopf.net/202006_lora_radio_node_v1.htm

## PINOUT
![image](https://github.com/user-attachments/assets/9a7414a0-1142-48fb-a367-c5485e6372cf)

# Adafruit 32u4 [CO2](https://github.com/OtagoPolytechnic/Cloudy-with-a-Chance-of-LoRa/wiki/CO2-Requirements)
Documented in [Issue #9](https://github.com/OtagoPolytechnic/Cloudy-with-a-Chance-of-LoRa/issues/9) \
How to connect:

## Arduino IDE:
Upload the Adafruit Board via this link: https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le/setup \
Under libraries in your IDE select Arduino AVR boards and install.\
There is some startup code at https://github.com/OtagoPolytechnic/Cloudy-with-a-Chance-of-LoRa/commit/3bc8c4a1a3944b14ba820b6ea63a7c25c0000499 \
In the arduino library under MCCI_LoRaWAN_LMIC_library/project_config/lmic_project_config.h comment out `#define CFG_us915` 1 and uncomment `#define CFG_au915 1` \
The library is at https://github.com/mcci-catena/arduino-lmic

In the code, the following needs updated:\
`static const u1_t PROGMEM APPEUI[8]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };`  //lmb\
`static const u1_t PROGMEM DEVEUI[8]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};` //lmb\
`static const u1_t PROGMEM APPKEY[16] = { 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0};` //msb\
These values can be found on The Things Network application under the device you added. 
* Change AppEUI to lsb 
* Change DevEUI to lsb 
* Change Appkey to msb 

![image](https://github.com/user-attachments/assets/3ee44c43-983a-4147-ba81-68e90ed52c6a)

Change these values to match the device you want to link it to. 

Go to File -> Preferences and select verbose output during upload.\
![image](https://github.com/user-attachments/assets/850b5fbd-3f15-4fcc-b436-5f7cf4df0f7d)

Then save and select upload to device.

Once the IDE says uploading... Press and hold the button on the Adafruit board for about 5 seconds. There should be a verbose output into the console. 
![image](https://github.com/user-attachments/assets/c245e02c-ab4a-491e-bf1f-36a5d25ce7e3)
![image](https://github.com/user-attachments/assets/987ff7b0-34bc-4150-be16-f5d3cca00ff6)

After this, the device should register in TTN.\
![image](https://github.com/user-attachments/assets/207d67d9-7091-46ac-9b09-6a3a23746afa)


# TTGO Lora

![H59eec206d363409686c79df88ff8dcb3V](https://github.com/user-attachments/assets/130a25fe-c690-4df0-b542-6d2148d44fc4)

In the arduino library under MCCI_LoRaWAN_LMIC_library/project_config/lmic_project_config.h add the line `#define hal_init LMICHAL_init` to the end of the file

## C02 Sensor Diagram
![c02](https://github.com/user-attachments/assets/1253e72f-0153-4c56-baf4-72942d7fea2b)

# XC3702 Barometric Pressure Sensor
Manual found [here](https://www.electusdistribution.co.nz/dbdocument/631671/xc3702_manual_13294.pdf)\
Library found [here](https://www.arduino.cc/reference/en/libraries/spl06-007/)\
![image](https://github.com/user-attachments/assets/c990593b-9e0a-47b2-b5e5-ef08e4122ff4)

# XC3780 Duinotech Arduino Compatible Dust Sensor
The small board has a SHARP GP2Y1014AU optical sensor onboard that uses an infrared LED and photoelectric transistor to detect reflected light from dust in the air. Fine particles as small as 0.8μm in diameter can be detected, including smoke particles, such as cigarette smoke.\
![image](https://github.com/user-attachments/assets/4531734f-bb2a-4621-9fe9-0197b89f4c5c)

The Duinotech Arduino Compatible Dust Sensor typically provides readings in micrograms per cubic meter (µg/m³). This unit of measurement quantifies the concentration of dust particles in the air.

# Duinotech Arduino Compatible Air Quality Sensor
https://www.jaycar.co.nz/duinotech-arduino-compatible-air-quality-sensor-with-co2-and-temperature/p/XC3782\
This air quality sensor can detect a wide range of volatile organic compounds (TVOCs), including equivalent carbon dioxide (eCO2) and metal oxide (MOX) levels. It also features a 10K precision 1% NTC thermistor, which can be used to measure the specific temperature in the environment. Equivalent carbon dioxide (eCO2) is measured in the range of 400 to 8192ppm (parts per million), and various volatile organic compounds (TVOC) ranges from 0 to 1187ppb (parts per billion). Includes 3mm mounting holes for fixed applications. 

Payload formatter:
```
function Decoder(bytes, fPort) {
    var co2 = (bytes[0] | (bytes[1] << 8)); // Combine low and high bytes for CO2
    var tvoc = (bytes[2] | (bytes[3] << 8)); // Combine low and high bytes for TVOC
    
    return {
        co2: co2,
        tvoc: tvoc
    };
}
```

# Weather Station Setup

## Master Slave Setup
The main weather station has 2 node devices. The master (TTGO) gets the data from the slave (feather) and the baramoter and uploads to LoRa. The slave has an I2C address of 0x08.

### Master

This is a TTGO esp32 module with a LoRa radio. Every 10 minutes it gets the data from the slave device and the baraometer, creates a payload and then sends it to TTN.

### Slave

This is a feather module. It uses the 2 inturrupts to keep track of the wind speed and the rain guage. The rain guage sends a pulse every 0.27mm of rain. This is sent as a number of pulses to TTN. This is reset every 12 hours.

The wind speed is also a pulse generator that generates 1 pulse per 2.4m/s of wind speed. It is averaged out and reset every time the master requests data (10 minutes).

