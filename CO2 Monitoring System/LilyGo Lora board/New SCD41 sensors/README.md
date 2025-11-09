# SCD-41 instructions

### How to solder the SCD-41

Using the PCB:
(Note: This is very hard and solder may not adhere to the correct pads so I would recomend soldering manually instead)
Add solder paste to all the pads, place the SCD-41 on top of the solder alligned in the square. 
![IMG_20251103_14294028](https://github.com/user-attachments/assets/23833dd1-9d8a-4e1c-9e42-277dc8e43867)

The SCD-41 has a little golden dot in one of the corners, this dot needs to be oriented with the white dot on the bottom left corner of the PCB:
![IMG20251009134300](https://github.com/user-attachments/assets/334af727-a91a-4fa3-884a-d8fb7b4f9615)

Using the heat gun apply even heat until you see the sensor sink onto the PCB, some solder may spill out and that can be removed when dried.

There is an error in the PCB design which requires one of the traces to be stratched out with a flat head screw driver before connecting to power as shown here: 
![IMG20251009141500-EDIT](https://github.com/user-attachments/assets/f3b6eb72-c56a-4e98-8cac-cd26f375d15d)

Lastly solder headers to holes then it should be good to go.


Soldering manually:
4 pads need to be connected using jumper cables.
The 4 middle pads and right side top pad need to be connected to each other (GND) (See photo)
Using the photo below this is how the SCD-41 needs to be soldered (The orange dot on the bottom right is where the golden dot is when fliped around).
![IMG_20251110_121439](https://github.com/user-attachments/assets/f1fdcc21-85e0-4ca0-b369-a648a3d353a4)


For note here is the documentation for the SCD-41 pads (We only need pads: 9, 10, 19, 20 and pads 21 and 6 connected together):
<img width="817" height="927" alt="image" src="https://github.com/user-attachments/assets/896ebfa6-550d-4f26-9b73-d9f47687b6ad" />


### How to use SCD-41 code

The SCD-41 code works with lilygo lora boards with power connected to 5V and SDA to pin 21 and SCL connected top pin 22:
<img width="600" height="450" alt="image" src="https://github.com/user-attachments/assets/40e6cb68-872d-4cea-a4c9-ca14460b1557" />
![1762730256873](https://github.com/user-attachments/assets/a502bbcf-593a-47ea-9133-ec3151784103)

The only changes to the code needed is the TTN unique device settings when creating a new TTN end device. These are stored in the config.h file of the arduino sketch.

### PCB and Gcode

This folder holds the designs for the PCB and Gcode to cut it using the woodpeckr.
