# UbuntuTrusty-mCard-loriot-AU915

## Ubuntu Trusty (14.04) with Lora mCard in a VirtualBox VM

## Preparation

- Install VirtualBox.
- In VirtualBox VM manager, install Ubuntu 14.04 (server or desktop)
- Plug Mcard/USB into PC
- Select virtual machine, click settings, click USB, add a USB device filter for FDTI device

## Configuration

- Install 32 bit ftdi drivers to talk to the mCard:

`sudo apt-get install libftdi1:i386`

- Install SSL 1.0.2, supplied by [3rd party repository](https://gist.github.com/mbejda/a1dabc45b32aaf8b25ae5e8d05923518)

```
openssl version -a
sudo add-apt-repository ppa:0k53d-karl-f830m/openssl
sudo apt-get update
sudo apt-get install openssl
openssl version -a
```

- Download the libmpsse source and compile/install

```
sudo apt-get install swig libftdi-dev python-dev
wget https://github.com/kellybs1/libmpsse/archive/master.zip -O libmpsse.zip
unzip libmpsse.zip
cd libmpsse-master
cd src
./configure
make
sudo make install
```

- Unplug card, power off VM, replug card, and power on VM
- The gateway is now ready for packet forwarding software

## Testing with Loriot server

- Open a terminal and get the Loriot binary address (it's different for every server, this gateway is registered on us1.loriot.io )

```
sudo mkdir /lrt
sudo chmod 777 /lrt
cd /lrt
wget http://us1.loriot.io/home/gwsw/loriot-linux-x64-mcard-USB-0-latest.bin -O loriot-gw
chmod +x loriot-gw
cd /lrt
sudo ./loriot-gw -f
```

## Notable Problems

- Without SSL 1.0.2 there will be an error
- Without install the i386 (32 bit) FTDI drivers there wiill be an error message about a missing .so file
- Gateway terminal will log 404 errors if gateway MAC is not properly  registered with Loriot (or other provider)
- There was an error message and LIBMPSSE code saying "you could be running an incompatible version of the binary" after installing another LIBMPSSE version. That version was replaced with the version above and after restarting the hardware the gateway was active
