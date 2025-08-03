# RaspberryPi-mCard-libloragw-AU915

## Step 1: Installing Raspbian Pi Jessie on a Pi 2

The first thing we want to do here is make sure that the SD card we are going to use with the Pi has been formatted to Fat32. We can just use SD card formatter found here: 
https://www.sdcard.org/downloads/formatter_4/eula_windows/index.html

Once the card has been formatted appropriately, we need to install the Raspbian Jessie OS onto it. Follow this tutorial here: 
https://www.howtoforge.com/tutorial/howto-install-raspbian-on-raspberry-pi/#-downloading-raspbian-and-image-writer

And to download the OS for the SD card:
http://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2017-07-05/

Note: To complete the tutorial you will have to install the OS on the SD card, then ensure the card is plugged into the Pi properly and the OS boots. Then you will need to hook the Pi up to a screen, keyboard and mouse, and follow the tutorial appropriately. 
Once the tutorial has been completed for a basic OS setup on the Pi, we can complete the rest of the necessary setup that will be specific to LoRa. Note: getting the raspberry pi to boot to a desktop GUI format is not necessary as we use the command line anyway.  These next steps will be:  

Optional Step (but probably a good idea)
Fixing keyboard layout becuase it is UK by default ([Source](https://www.raspberrypi.org/forums/viewtopic.php?f=28&t=80127)). Run the following command:

```
sudo raspi-config
```

Use Tab select select OK/Cancel:

•	Choose 4: Localization
•	Choose I3: Keyboard setup
•	Choose generic 102 keyboard
•	Choose Other
•	In country of origin menu choose English (US)
•	Then from Keyboard layout menu choose English (US)

## Required Steps:

### Enable SSH

While in raspi-config, go back to main menu:

- Select Interfacing Options
- Select P2 SSH
- Choose Yes
- Select OK
- Choose Finish

Now reboot:

```
sudo reboot now
```

Note: To then SSH into the Pi, use the inet address found with ifconfig, then ssh with putty into that addr and use port 22. 

Actually setting up mCard / GateWay now. Bump the USB power and enable SPI on the Pi ([Source](https://github.com/mirakonta/lora_gateway/wiki/Part-2:-Hardware-modifications)). Raspberry-Pi 2 has USB current limiter that could prevent the concentrator to start up correctly. Execute the following steps:

```
sudo nano /boot/config.txt
```

Add to this file:

```
max_usb_current=1
```

Add or uncomment:

```
dtparam=spi=on
```

Reboot the Raspberry Pi. Now, check SPI working with the following command:

```
ls /dev/spi*
```

Where the response should be:

```
/dev/spidev0.0 /dev/spidev0.1
```

#### Set up and install libftdi drivers

Run to make sure all software is up to date:

```
sudo apt-get update
```

Install git so we can grab a file from an exisiting repo:

```
sudo apt-get install git --assume-yes
```

Install necessary drivers for the LoRa MCard

```
sudo apt-get install libftdi-dev --assume-yes
cd ~
```

Download tar file file:

```
wget https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/libmpsse/libmpsse-1.3.tar.gz
```

Extract tar file (the libmpsse library is an open source library for SPI/I2C control via FTDI chips):

```
tar zxvf libmpsse-1.3.tar.gz
```

cd into extracted libmpsse source code directory:

```
cd libmpsse-1.3/src
```

Responsible for getting ready to build the software on your specific system:

```
./configure --prefix=/usr --disable-python
```

We can invoke _make_ to build the software. This runs a series of tasks defined in a _MakeFile_ to build the finished program from its source code:

```
make
sudo make install
```

Udev is the device manager kernel that creates/removes device nodes in the /dev directory dynamically. Here we reload the rules without restarting the udev:

```
sudo udevadm control --reload-rules
```

Now add the user _pi_ to the _plugdev_ group:

```
sudo adduser pi plugdev
```

Set up and install gateway and packetforwarder ([Source](https://github.com/Lora-net/lora_gateway
https://github.com/Lora-net/packet_forwarder/wiki/Use-with-Raspberry-Pi)), using the following instructions:

```
cd ~
```

Make a lora directory, with an exec directory in it:

```
mkdir lora
mkdir ~/lora/exec
```

Change into the lora directory:

```
cd ~/lora
```

Clone both github repo forks with the needed modifications to be able to use lora_gateway with a LoRa Mcard:

```
git clone https://github.com/kellybs1/lora_gateway.git
git clone https://github.com/kellybs1/packet_forwarder.git
```

Change into the lora gateway repo and the packet forwarder repo. We then run make clean all to ensure we have clean builds in both these repos and no leftover files from previous builds:

```
cd ~/lora/lora_gateway
make clean all
cd ~/lora/packet_forwarder
make clean all
```

Copy the executable:

```
cp ~/lora/packet_forwarder/basic_pkt_fwd/basic_pkt_fwd ~/lora/exec/
```

Get AU915 config used by Dunedin IoT group:
```
wget -O ~/lora/exec/global_conf.json https://gitlab.op-bit.nz/BIT/Project/Internet-Of-Things/gateways/raw/master/global_conf.json
```

Change the gateway ID:

```
sudo cp ~/lora/packet_forwarder/basic_pkt_fwd/local_conf.json ~/lora/exec/local_conf.json
sudo nano ~/lora/exec/local_conf.json
```

Good idea to use the MAC address for this as it’s unique-ish. See MAC address by typing:

```
ifconfig
```

Change _gateway_ID_ (e.g., `gateway_ID: B827EBFFFEE2AF08`), which is what the gateway has been set to as this was the default MAC address.

#### Optional: Change network server address and ports

These values should already be set if using DunedinIoT global config file so may not be necessary ([Source](https://github.com/mirakonta/lora_gateway/wiki/Part-5:-LoRaWAN-Network-Server)). Use the following commands:

```
sudo nano ~/lora/exec/global_conf.json
```

Go down to bottom section called _gateway_conf_. Change _server_address_, _serv_port_up_ and _serv_port_down_ as appropriate. 

#### Running the gateway

To run gateway execute:

```
cd ~/lora/exec
sudo ./basic_pkt_fwd
```

#### Optional: Save the Pi image
https://thepihut.com/blogs/raspberry-pi-tutorials/17789160-backing-up-and-restoring-your-raspberry-pis-sd-card

#### Optional: Auto login on Raspbian Jessie
Source: https://www.raspberrypi.org/forums/viewtopic.php?f=28&t=127042

```
sudo touch /etc/systemd/system/getty@tty1.service.d/autologin.conf
```

and put this in it:

```
[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin pi --noclear %I 38400 linux
Then run
sudo systemctl enable getty@tty1.service`
```

## All Sources:

- https://www.sdcard.org/downloads/formatter_4/eula_windows/index.html
- https://www.howtoforge.com/tutorial/howto-install-raspbian-on-raspberry-pi/#-downloading-raspbian-and-image-writer
- http://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2017-07-05/
- https://sourceforge.net/projects/win32diskimager/
- https://www.raspberrypi.org/forums/viewtopic.php?f=28&t=80127
- https://www.raspberrypi.org/documentation/remote-access/ssh/
- https://github.com/mirakonta/lora_gateway/wiki/Part-2:-Hardware-modifications
- https://github.com/Lora-net/lora_gateway
- https://github.com/Lora-net/packet_forwarder/wiki/Use-with-Raspberry-Pi
- https://github.com/mirakonta/lora_gateway/wiki/Part-5:-LoRaWAN-Network-Server
- https://thepihut.com/blogs/raspberry-pi-tutorials/17789160-backing-up-and-restoring-your-raspberry-pis-sd-card
- https://www.raspberrypi.org/forums/viewtopic.php?f=28&t=127042
- https://kuziel.nz/notes/2016/08/lora-gateway-mk1-raspberrypi-multitech-lora-mcard.html
