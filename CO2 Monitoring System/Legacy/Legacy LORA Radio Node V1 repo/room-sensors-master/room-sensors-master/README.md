# Welcome to the room sensors repo!

## Folder breakdown:
 - **3D**, STL's and design files for our 3D printed enclosures
 - **node**, Device code and wiring description for the device itself
 - **web**, Our web application

# All the following instructions are made for Windows 10 OS

## The Flow of this application

**This application received raw payload directly from TTN.  We are not using database from our ingest.**

1. Temperature and humidity devices together send data to TTN under op_roomsensors application.  Separately co2 devices send its data to TTN under op_roomsensors application.

2. `mqtt.py` listens to TTN. The on_message function receives data from TTN and map data from sensor with api_sensor table. This gets saved to the local database into table "api_sensor" as "hardware_serial : xxxxx, name: Peter, temperature: 23deg, humidity: 60.5%, time : 2020-06-03 10:02:22, co2_ppm: null".  Data from room sensor will record co2_ppm as null, while data from co2 sensor will record temperature and humidity as null.  History is recorded data from each node on separate record.  

3. The React App runs in the browser and asking the server (python/django) for the current temperatures in all rooms via the REST API.

4. Server queries the local database called "api_sensor" table and sends it back to the frontend.

5. Frontend (React) receives data from local sqlite3 database and displays it in the bubble.


## Room sensor Project progress during S1, 2020

- Migrated from the old gitlab.op-bit to gitlab.com/iotop
- Progress on this projects have been pushed to dev branch.
- Created and deployed a development servers for Room Sensor.
- Improved documentation on deployment and code comments.
- Completed reversed proxy for dev server so all applications are running from the same dev server under different docker containers.  Our dev web application can be accessed via http://10.25.138.157:8001/ after adding additional allowed host inside local /etc/hosts file.
- Integrated co2 application to Room sensor on web application as well as on devices to TTN.
- Displays Co2 value and timestamp on web application.
- Improved all security issues within the project by using Python module and .env.
- Use Wiki for all security issues and solutions as well as documentation.
- Use SQLite3 to add extracted data from co2 as well as room sensor via mqtt.py.
- CO2 gauge project has been developed during this semester, which can be integrated to the project when completed.
- All sensors used for Heatmap/Co2 application could be found in Sensitive Wiki Repo (https://gitlab.com/iotop/sensitive/-/wikis/Sensors-Information).

