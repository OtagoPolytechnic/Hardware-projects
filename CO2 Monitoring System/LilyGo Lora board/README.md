# Table of Contents
* [Connecting LilyGo to TheThingsNetwork](#connect-lilyGo-to-theThingsNetwork)
  * [Prerequisites](#prerequisites)
  * [Step 1: IDE Setup](#step-1-ide-setup)
  * [Step 2: TTN Setup](#step-2-ttn-device-setup)
  * [Step 3: Code](#step-3-code)
* [:construction: CO2 Software Instructions (to be added)](#-co2-software-instructions-to-be-added)
* [:construction: Adding Device to the App (to be added)](#adding-device-to-the-app-to-be-added)
* [Troubleshooting](#troubleshooting)

# Connect LilyGo to TheThingsNetwork

The instructions below are steps on how to set up a LilyGo module and connecting it to TTN.

## Prerequisites

**Components:**
- T3 V2_1.6.1 LilyGo board
- USB micro B cable (old android charger)
- PC with ArduinoIDE installed

1. **You will need your own TheThingsNetwork (TTN) account for development and testing purposes**. These instructions will assume you're using your own account. 

2. **If you're using the specified board in the components list**, [skip to step 1](#step-1-ide-setup). 

If you're using a different model, you will need to find the CS, DIO/IRQ (interrupt pin), RST, and a to be used as additional interrupt OR a GPIO pin. Google your board model + "pin out" to ensure you've got the right ones.

## Instructions

### Step 1: IDE Setup

1. In the ArduinoIDE, click the **second icon in the left-hand menu** (Boards Manager) or select **Tools -> Board: -> Boards Manager…**, then search **esp32** in the search field, select the latest releases and click install.

<div align="center">
<img src="https://github.com/user-attachments/assets/a5dac74a-46bf-4fcb-8bae-97d6fe66ac76" width="400"/>
</div>

<hr/>

2. Then, Open **File -> Preferences**, then click the icon next to the **"Additional Boards Manager URLs"** section and paste the following: 

`https://espressif.github.io/arduino-esp32/package_esp32_index.json`

<div align="center">
<img src="https://github.com/user-attachments/assets/1a4baff0-88b4-42dc-aa5c-4465f3f73dc6" />

<img src="https://github.com/user-attachments/assets/d2f2cc42-d5c3-4816-bf24-4d6654b978e1" width="500"/>
</div>

<hr/>

3. Connect the board to your computer with a micro USB cable. Click the **Select Board drop down** (or go to the tools menu), select your COM port, **Click Select Other Board and Port -> Search "lora32" -> Select "TTGO LoRa32-OLED" -> Click OK**

<div>
<img src="https://github.com/user-attachments/assets/bda49a06-efb2-4f31-9bf5-e27cf5e9f02a" width="600" />
</div>

<hr/>

4. Click **Tools -> Board Revision -> TTGO LoRa32 V2.1 (1.6.1)**

<div align="center">

<img src="https://github.com/user-attachments/assets/e4e7e1e5-8b16-4823-a26b-ac2db777f638" width="500"/>

</div>

<hr/>

### Step 2: TTN Device Setup

Ensure you're logged in to _your personal TTN account_. Make a new application with a relevant name e.g. "co2-sensor-test".

> **Note:** you DO NOT need to make a new application if you're using the opiot account. You will still need to make a new device within the selected application though.

1. Within the selected application, navigate to **End Devices -> Register End Device**. 

<hr/>

2. In the **End device type section, under Input Method**, select the `Enter end device specifics manually option`.

<img src="https://github.com/user-attachments/assets/5e447ab5-62c5-4019-aae0-4e50af358924" width="400"/>

<hr/>

3. Configure the following details:

* LoRaWAN version: **LoRaWAN Specification 1.1.0**
* Regional parameters version: **RP001 Regional Parameters 1.1 revision B**

For the **Provisioning information**, enter the following details:

* JoinEUI: 0000000000000000
* DevEUI, AppKey & NwkKey: **Generate by clicking the Generate button.**

After configuring your device, **select the Register end device button**.

<hr/>

4. Done! Now in your new device, head to the **Payload Formatters tab -> Use custom JavaScript formatter**

Copy and paste this in:

```C++
function decodeUplink(input) {
    var value1 = input.bytes[0];
    var value2 = (input.bytes[1] << 8) | input.bytes[2];
    return {
        data: {
            value1: value1,
            value2: value2
        }
    };
}
```

<div align="center">

<img src="https://github.com/user-attachments/assets/02a6ac97-fe54-49bd-bf22-109c83b3481e" width="700"/>

<img src="https://github.com/user-attachments/assets/5d0e133a-e930-4555-b563-e4a947ef0244" width="700"/>

</div>

<hr/>

6. **Don't forget to press "Save"!!**

### Step 3: Code

Now you need to get the code!

1. **Create a new sketch**. Then navigate [here](https://github.com/OtagoPolytechnic/Air-Quality-Monitoring-System/blob/liz-lilygo-work/hardware/Connect_TTN_OTAA/Connect_TTN_OTAA.ino) and **copy the code** into your sketch.

**LEGACY DEVICE CODE** https://drive.google.com/drive/folders/1GD2Gc_5FPA3mIOz5uPzMBbV_UnKvAdes?usp=sharing

<hr/>

2. **Save** your sketch (name it something descriptive that you'll remember!) and **create a new file in the _same_ sketch folder** called `config.h`.

<hr/>

3. **Open that config file in your Arduino IDE** (_make sure your .ino file is also open_) and navigate [here](https://github.com/OtagoPolytechnic/Air-Quality-Monitoring-System/blob/liz-lilygo-work/hardware/Connect_TTN_OTAA/config.h), and **copy the code into your config file**.

<hr/>

4. **Edit the TTN keys (Lines 15-25)**. To get your keys, go into your **new device's TTN page -> Device Overview tab**, and scroll to the **"Activation Information"**. **Copy your JoinEUI and DevEUI** as is into the corresponding variable. For DevEUI, **keep the `0x` in front of the value.**

For **AppKey and NwkKey, click the eye icon, then the </> icon**, and then copy and paste into corresponding variable.

It should look like this:

<div>
<img src="https://github.com/user-attachments/assets/c71c5e00-47ff-4a06-9878-b9975f29f07a" width="800"/>
</div>

<hr/>

5. Check that line 8 in your config file looks like this:

<div>
<img src="https://github.com/user-attachments/assets/089e2fb0-c1b5-4aed-a562-f189d6caf102" width="700"/>
</div>

<hr/>

### Upload the code

1. Upload code to your module. Then, open your Serial Monitor. You should see it fail to join at first and succeed on the next try.

2. Head to the `Live Data` tab in your TTN console and you should see your LoRa module connecting and sending 2 random numbers per uplink.

Your data should look something (not exactly) like this:

<div>
<img src="https://github.com/user-attachments/assets/54520a9b-ca9f-4dde-9638-0bc911627617" width="700" />
</div>

<hr/>

# 🚧 CO2 Software Instructions (to be added)

<hr/>

# 🚧 Adding Device to the App (to be added)

<hr/>

# Troubleshooting
* `DevNonce is too small`
  * If you get this issue head to your end device settings:
![settingsTTN](https://github.com/user-attachments/assets/bfaae824-6b49-4f4c-97bb-66eec2c00571)
Then head to Join Settings, click expand and then tick the setting ``Resets join nonces``:
![joinNonce](https://github.com/user-attachments/assets/da2e9725-b617-4b6f-afb3-a220b6d3a4a3)
Hit save settings down the bottom and see if your device connects.
If your device connects you can then uncheck that setting and all should work from there.

***

***

Hardware Technology Choices (Legacy devices)

***

Here we list the technologies used and justify why we chose to use them.

Research here: https://docs.google.com/document/d/10VE9JVAf2EToZlwPcisKHPzI0QM7LUKEtz-x0vRxqYc/edit?usp=sharing

# LoRa Board
Our project uses LoRaWAN technology to communicate with TheThingsNetwork and our API service. To do this, a device that has LoRa capabilities is needed to send and receive data for LoRa-based communication.

The previous CO2 team used a LoRa Radio Node TB:IOTMCU which is a more compact, single-purpose board designed for LoRaWAN. However, at the end of their term, they concluded that using a TTGO LoRa32 board would be better for the project as it is a more feature-rich, versatile board with an ESP32, OLED display, WiFi capabilities, and is easy for debugging and data visualization.

We've been using the LilyGo TTGO LoRa32 T3 V2_1.6.1 923mHz model (TTGO161 for short) as these were the models provided for us based on the research done by the previous team. We've found it easy to connect to TTN using these boards even with limited documentation.

However, the TTGO161 model is quite expensive and not commonly used compared to other TTGO boards. With this in mind, we researched models that were cheaper, just as effective, well documented, compatible with other technologies used, and easy to plug-n-play when substituting the TTGO161. We're still undergoing research at the moment, but most options are at least $25 for one unit.

We'd like to use our current model, but may have to revert back to using the TB:IOTMCU Radio Nodes if they're cheaper, despite the limited functionality.

# CO2 Sensor
In-depth research was performed to understand how CO2 works, how to measure it, and different model options for sensors. For more info, read about our [CO2 Assessment & Strategies research](https://docs.google.com/document/d/10VE9JVAf2EToZlwPcisKHPzI0QM7LUKEtz-x0vRxqYc/edit?usp=sharing).

The sensor used by previous teams has been the MHZ19B, an NDIR CO2 sensor. This sensor reads CO2 data and has worked as intended. However, according to our research, while relatively inexpensive, has some drawbacks, including potential inaccuracy at low CO2 levels, sensitivity to other gases, and a need for proper calibration and handling to avoid damage. Additionally, using an unstable power source can cause the sensor to malfunction or break, which is not suitable for how the sensors have been set up (power off when PCs are turned off). We also use its temperature sensor readings to display temperature on the website, which is not ideal as these temp readings are for sensor calibration purposes only and not an accurate measure of room temperature. These sensors have been inconsistent, unstable, and difficult to troubleshoot in our experience, so a new sensor is needed.

We decided on using either an NDIR (Non-Dispersive Infrared) or PAS (Photo-acoustic Spectroscopy) type sensor as these sensors offer the accuracy and price range desirable for our project. After further research, we decided on a model from the SCD4x range. Our ideal sensor the SCD41 sensor as it offered a higher reading range and better accuracy compared to the SCD40 model, but is more expensive. 

So, the SCD40 is the model we decided on as it met the requirements for our use case, offers a decent price, more stability and data consistency, and does not require manual calibration/handling.
