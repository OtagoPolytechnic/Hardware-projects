# TinyLoRa Sketch for Adafruit Feather 32u4 LoRa RFM9x

### These instructions follow on from the ones found [here](https://gitlab.op-bit.nz/BIT/Project/Internet-Of-Things/nodes/blob/master/AdafruitFeather32u4-ABP-ClassroomSensor-AU915/README.md)  , but with a new sketch.

## Preparation

- Download and install the Adafruit TinyLoRa library. You can [download it from here](https://github.com/adafruit/TinyLoRa).
- Download and install the Adafruit SleepyDog library. You can [download it from here](https://github.com/adafruit/Adafruit_SleepyDog).
- Download and install the RadioHead Packet Radio library. You can [download it from here](https://github.com/hallard/RadioHead).

## Adding the Example Sketch 

- Add the provided sketch for the Adafruit Feather 32u4 from the `TinyLoRa` folder into the Arduino Sketchbook folder.
- Make sure to put the sketch (`.ino` file) into a folder with the same name. If you forget, the Arduino software will do this for you. 
- Open the sketch for your device
- Set the ABP keys!

You need to replace the `FILLMEIN` placeholders with a valid Network Session Key (NWKSKEY), Application Session Key (APPSKEY) and Device Address (DEVADDR). The block of code is displayed for reference below.


```
// Visit your thethingsnetwork.org device console to create an account, or if you need your session keys.
// Network Session Key (MSB)
uint8_t NwkSkey[16] = { FILLMEIN };

// Application Session Key (MSB)
uint8_t AppSkey[16] = { FILLMEIN };

// Device Address (MSB)
uint8_t DevAddr[4] = { FILLMEIN };
```

By default, the TinyLoRa libray is set to US broadcast specifications, you'll need to navigate to your Arduino library folder (`H:\My Documents\Arduino\libraries\TinyLoRa-master`) and edit the TinyLoRa.h file. 

Comment out the line "#define US902" and uncomment "#define AU915", when finished your file should look something like this. 

```
/** Region configuration*/
//#define US902 ///< Used in USA, Canada and South America
//#define EU863 ///< Used in Europe
#define AU915 ///< Used in Australia
//#define AS920 ///< Used in Asia
```

You're done! Your sketch is ready to be compiled and uploaded by clicking on _Upload_. Then wait until it shows _Done uploading_ at the bottom


## Checking Functionality

- Make sure you have the correct COM port selected
- Make sure you have the right board selected
- Check if the Serial Monitor is set to `9600`
- Click on _Serial Monitor_ when _Done uploading_ is shown at the bottom
- You should see output similar to the listing below:


```
Open to new sketch for 10 seconds!
Starting LoRa... OK

TX_INTERVAL: 600 sec

Sensor values:
Battery Voltage: 4.36 V
Humidity: 37.80 %
Temperature: 23.90 C

Sending LoRa Data...
Serial will die now. Don't expect more output. 
```

## Understanding the payload

The payload sent by this sketch is reasonably simple, only 6 bytes in total containing temperature, humidity and battery voltage information. 

In order to decode the payload we should first figure out how the sketch encodes this information. Let's assume the following values:

- Temperature: 23.50 C
- Humidity: 37.10 %
- Voltage: 4.31 V

The sketch will multiply these values by 100, rounding off any decimal values and then store these values as 16-bit integers. 

For example, `tempInt` becomes 2350, `humidInt` becomes 3710 and `battInt` becomes 0431, this eliminates the need for a decimal point. 

The sketch then combines these values together to build the payload! The first two 8-bit bytes being temperature, the second two being humidity and the last two being voltage. 

For example, assuming the above values our 6 bytes would looks like this: 
```
// Temperature
loraData[0] = 00001001;
loraData[1] = 00101110;

// Humidity
loraData[2] = 00001110;
loraData[3] = 01111110;

// Battery
loraData[4] = 00000001;
loraData[5] = 10101111;
```

All packed up, these 6 bytes would look like this "00001001 00101110 00001110 01111110 00000001 10101111" or "09 2e 0e 7e 01 af"

LoraServer has a habit of encoding payloads as BASE64 upon arrival, which would look even more complicated (CS4OfgGv) however this is reasonably simple to reverse.

As a basic example, to decode 'Temperature' we would take the first two bytes of our payload (00001001 00101110), which in it's pure decimal format is '2350'. 
That decimal value divided by 100 gives us our original reading of 23.50!


# The Things Network Decoder

The things network allows you to set a payload formats for information decoding on arrival. Ours would look like this. 
```
    // TinyLoRa - DHT22 Decoder
    function Decoder(bytes, port) {
      var decoded = {};
     
      // Decode bytes to int
      var celciusInt = (bytes[0] << 8) | bytes[1];
      var humidInt = (bytes[2] << 8) | bytes[3];
      var battInt = (bytes[4] << 8) | bytes[5];
      
      // Decode int to float
      decoded.celcius = celciusInt / 100;
      decoded.humid = humidInt / 100;
      decoded.batt = battInt / 100;
     
      return decoded;
    }
```

Or if we are using the legacy payload style, it would look like this. 
```
    // TinyLoRa - Legacy Payload Decoder
    function Decoder(bytes, port) {
      var decoded = {};
     
      // Split bytes from string
      var celciusSlice = String.fromCharCode.apply(null, bytes.slice(3, 8));
      var humidSlice = String.fromCharCode.apply(null, bytes.slice(15, 20));
      var battSlice = String.fromCharCode.apply(null, bytes.slice(26, 30));
      
      // Convert string to float
      decoded.celcius = parseFloat(celciusSlice);
      decoded.humid = parseFloat(humidSlice);
      decoded.batt = parseFloat(battSlice);
     
      return decoded;
    }
```