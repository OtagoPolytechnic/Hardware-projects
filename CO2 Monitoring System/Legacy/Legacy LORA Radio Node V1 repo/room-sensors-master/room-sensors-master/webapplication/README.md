### This README is dedicated to instruction and how to run web applications for development and Production

## Table of contents
- [Development](#Development Section)
    * [Preparation](#preparation)
        + [Cloning Project](#Cloning Project)
        + [Set up virtual environment for your project](#Set up virtual environment for your project)
        + [Django environment using PyCharm](#Django environment using PyCharm)
        + [Django environment using Anaconda Navigator](#Django environment using Anaconda Navigator)
    * [Installing project](#Installing project)
        + [Django part (backend)](#Django part)
        + [React part (frontend)](#React part)
    * [Possible Issues/Solutions](#Possible Issues)
        + [NPM error](#NPM error)
        + [Housekeeping for Server during Development](#Housekeeping for Server during Development)
        + [SQLite using simulated data from TTN for development purpose](#SQLite using simulated data from TTN for development purpose)
        + [Using SQLite3 with Room Sensor Project with DB Browser for sqlite3](#Using SQLite3 with Room Sensor Project with DB Browser for sqlite3)
        + [Housekeeping on front end](#Housekeeping on front end)
        + [Potential errors during testing on MQTT handler](#Potential errors during testing on MQTT handler)
- [Deployment](#deployment)
    * [Deployment - New environment (Ubuntu 18.04)](#newdeploy)
        + [Django](#Django)
        + [Node](#Node)
        + [Docker](#Docker)
        + [SQLite3 in Deployment](#SQLite3 in Deployment)
    * [Deployment - Production server (Ubuntu 16.04)](#olddeploy)

<a name="Development Section"></a>
# Development Section

**Note** Prerequisite to develop this project includes all packages inside - requirements.txt but also make sure your local python version is 3.6 or above in order to avoid any potential unnecessary errors.

<a name="preparation"></a>
## Preparation
<a name="Cloning Project"></a>
### Cloning Project
 
1.  Navigate to your local project folder and make sure you have initialise this folder as your git repository folder.  It is also important to have SSH access to team repo in order to perform `git clone git@gitlab.com:iotop/room-sensors.git`.  Please contact your team leader if you do not have access via SSH.


2. After cloning repository, open a web browser to copy content of `secrets.py` from `https://gitlab.com/iotop/sensitive/-/tree/master/Roomsensors` - `secrets.py` is to be placed inside `yourprojectfolder/webapplication/heatmap` folder.  It contains sensitive information for Django to run when `python manage.py runserver` is used.  It also contains credential for MQTT subscription.


*NOTE :* Please ensure the correct `TTN_USERNAME` AND `TTN_PASSWORD` are corresponding to application you are testing on as there are 3 different credentials within this file.

<a name="Set up virtual environment for your project"></a>
### Set up virtual environment for your project

You can select your preferred method of virtual environments; here are some instruction for Pycharm and Anaconda.

<a name="Django environment using PyCharm"></a>
### Django environment using PyCharm
1. Open a PyCharm and go to File>NewProject
2. Enter the intended location (e.g. H:\Project\heatmap)
3. Click Project Interpreter: New Virtualenv environment
4. Check the correctness of the location and name your virtual environment folder (e.g. H:\Project\heatmap\venv)
5. Choose the correct Base interpreter (e.g. C:\Program Files (x86)\Python36-32\python.exe) and click Create

After that, whenever you open this project folder in PyCharm, and open a terminal, 
the "(venv)" should precede the location of the project folder. That means all your commands in that terminal 
will be run within a virtual environment and you can start installing packages safely without altering the global
scope. If that doesn't happen, try the following:

In the Terminal after your project's location enter "~name of the venv folder~\Scripts\activate".
It should look like "H:\Project\heatmap>venv\Scripts\activate"

<a name="Django environment using Anaconda Navigator"></a>
### Django environment using Anaconda Navigator
1.  Ensure you have Anaconda Navigator in your machine.  Open Anaconda Navigator, select Environments on the left tab.
2.  Click 'Create' and enter name you would like to use for this project virtual environment.  Click on 'packages' and select Python 3.6 or more.  Click 'Create'.
3.  After the creation of virtual environment, it will appear in the list below base(root) option.  Select your newly created environment and after it finishes loading, left click on the triangle icon.  This should bring up two options; select Open Terminal and navigate to your local development folder.    

<a name="Installing project"></a>
## Installing project

If your local python is below 3.x - you will be required to replace 'python' with 'python3' along the way.

<a name="Django part"></a>
### Django part (backend): 
1. Get the project from the working repo (read the git documentation on the web) into your main project folder.

2. CD into the "webapplication" directory (e.g "Documents\IOT\room-sensors\webapplication").  Run "pip install -r requirements.txt" in the PyCharm terminal/Virtual environment terminal. That will install all the required packages into your virtual environment.

3. Run "python manage.py migrate" to initially set up the database

4. If you install any additional packages, run the following command in the PyCharm terminal:
"pip freeze > requirements.txt". This will add the packages to the requirements file, so that anyone else who downloads the project can install the required packages too.

<a name="React part"></a>
### React part (frontend):
1. Download and install node.js from the web.

2. CD into the "front-end" directory in the terminal from a different terminal (like gitbash).

3. Run "npm install".

4. Run "npm build".

5. Run `npm install moment --save` and `npm install dotenv --save` to install moment and dotenv packages in order to use time and `.env` file inside frontend application.  To check if the package is installed properly, go to `package.json` file and seeing `dotenv` and `moment` in the dependency list.

6. Run "npm start" this will run on port: 3000. Make sure these commmands were run in the "front-end" directory ("room-sensors\webapplication\front-end").

7. Copy `.env` file from 'https://gitlab.com/iotop/sensitive/-/tree/master/Roomsensors' and paste into `/webapplication/frontend`, which is the root npm folder.

**Note** :  Make sure to comment out the accurate variable as required; there are URLs for development and production purposes in this file.

8. Go back to Django terminal and CD into the project folder and run "python manage.py runserver". This should run the Django project and you should be able navigate to http://localhost:8000 and see a blank page as Heatmap is using React on the front end but running and being served by Django, which controls and interacts with your incoming data on the backend.   You should be able to view the project front end on http://localhost:3000. 

<a name="Possible Issues"></a>
## Possible Issues/Solutions

<a name="NPM error"></a>
### NPM error:
If you get an error that npm command is not recognized make sure node.js is added to your environment veriables:

1. Make a global OS search for "environment", open the Edit the system environment variables and press
Environment variables in the Advanced tab of the System Properties

2. Press New under the User variable for ~your-user-name~

3. Name your variable somehow (e.g. Path) and add a path to your nodejs installation (e.g. C:\Program Files\nodejs\)

4. Restart all the terminals and the problem should be fixed


<a name="Running it locally without ttn and access to main database"></a>
### Running it locally without ttn and access to main database 

To run and access local database locally you will need to 

1. Change API url under `/webapplication/front-end/.env`.

2. Uncomment the local API host and comment remote URL.

```bash
# Local Environment
REACT_APP_SENSORLINK = http://localhost:8000/api/sensors/<br>
REACT_APP_CREDLINK = http://localhost:8000/api-auth/<br>
```

3. You can keep the database settings in settings.py as they are in sqlite or change them (scroll down the page to find DATABASES), you will need to populate your database after running python manage.py migrate.  Example for setting up mysql https://stackoverflow.com/questions/19189813/setting-django-up-to-use-mysql on settings.py


<a name="Housekeeping for Server during Development"></a>
### Housekeeping for Server during Development

 1. Ensure that you have data in your database. Copy the db.sqlite3 file from the production server and copy it into your /webapplication folder.

 2. Not seeing data when you visit localhost:3000? Open up the web console and check for CORS errors. Make sure to use Firefox when you do this as Chromes errors are ambiguous when this occurs. If you are seeing CORS errors, visit your settings.py file and ensure there is no weird tabbing or missing commas, and that you have http:// infront of your IP address & localhost:3000 in CORS_ORIGIN_WHITELIST. 

 3. If you make changes to your utils file make sure to use npm run-script build. If for some reason it doesn't seem to be updating you can delete the build folder as this command will recreate it.

 4. If `npm run-script build` ended with `fails to minify` error.  Go to `/front-end/src/App.css` to comment out `require('dotenv').config();` and re-run the command.

<a name="SQLite using simulated data from TTN for development purpose"></a>
### SQLite using simulated data from TTN for development purpose

You could use MySQL to work with this project but our current setting (Semester 1, 2020) is using SQLite3.  You can simulated incoming values from TTN for your application even without having hardware sending actual data by following instruction below :

1.  Visit our database server using the link and credential in sensitive repository and use select table called `data`.

2.  To get your values to test with TTN, use select statement to extract only data from `op_roomsensors` and used the dated data, which may be in an unreadable format like `VDogMjQuMzAqQy`, which is in base64 format.  Convert into Hex values by using online tool such as `https://base64.guru/converter/decode/hex`.

3.  Log in to  `https://console.thethings.meshed.com.au/applications/roomsensor_dev`.  This application is used as a development TTN application so that our database server is unaffected by the testing.  Select a device in the list and scroll down to `Simulated Uplink` and paste hex values from the previous step to Payload field and send.  

![uplink](Img/uplink.png)

4.  The simulated data should appear on Data tab.  

![data](Img/data.png)

5.  If you are testing to send data from multiple applications like room sensor sensors and co2 sensors for this application, you will need to set FPort for each application input and use Payload Formats to create multiple functions in order to receive different type of payload_fields.  These values will be sent as 'payload_fields' in JSON objects in MQTT handler.  From the picture, it can be noticed that different payload fields are generated for two different applications.  Payload fields may not appear correctly if used the wrong port when inserting values. The current payload format functions for this development application can be found `https://console.thethings.meshed.com.au/applications/roomsensor_dev/payload-formats`.  


![unifiedports](Img/unifiedports.png)

<a name="Using SQLite3 with Room Sensor Project with DB Browser for sqlite3"></a>
### Using SQLite3 with Room Sensor Project with DB Browser for sqlite3

There were several migrations issues using MySQL so here is some information regarding the use of SQLite3 to test your project.

1.  Download and install in your local computer DB Browser for SQLite from `https://sqlitebrowser.org/`

2.  Open the application virtual environment terminal, use command `python manage.py sqlmigrate api 0001`. This will migrate 0001 migration file into our api application.  Do for all migration files inside migration folder to complete migrating all requirement. Remember to first run `python manage.py migrate` BEFORE running these migrations. More information on sqlmigrate can be found on `https://docs.djangoproject.com/en/3.0/ref/django-admin/#django-admin-sqlmigrate`.  After migration, you will find a newly created file called `db.sqlite3` inside `webapplication` folder. 

3.  Check sqlite3 file by opening DB Browser for SQLite and import db.sqlite3 file.  

4. A SQLite3 glitch with `no api__old_table issue` was found during development, which can be fixed by temporary editing History table in `api/models.py`.  Change humidity `null=True` to `default=0.00` and makemigrations >> migrate >> sqlmigrate >> runserver.  However, please remember to make run all these commands to revert value back to `null=True` or database may fail to save.

<a name="Housekeeping on front end"></a>
### Housekeeping on front end

There are several common errors when first using this application.  Here are some steps to avoid issues: 

1. Change TTN subscription in `secrets.py` to the right application using TTN application Access Key. Check `webapplication/frontend/.env` to ensure it is redirecting to the accurate API host. 

2. Check payload conversion to ensure it is decoded correctly especially if the MQTT handler displays any input decoded errors as payload may not need to be decoded.   

3. Create a record and filled your database fields with correct data types.  These fields will be updated to the correct values if done properly.  You may need to close your DB SQLite browser to view the updates.  Make sure the serial number in DB is corresponding to the hardware_serial of the device from TTN as mqtt.py will search from local database using that as ID to search for incoming sensor data.


<a name="Potential errors during testing on MQTT handler"></a>
### Potential errors during testing on MQTT handler

1. `Unicode error 'utf-8' codec can't decode byte 0x9c in position 1: invalid start byte` - indicate the CO2 variable has arrived decoded; therefore, removing any base64.b64decode method will fix the problem.

2. `Sensor query matching does not exist` error - indicate there is no matched record based on your request from mqtt.py.  Make sure there is a record in local database that matched the request.

3. Not receiving data in MQTT handler - may indicate incorrect TTN application configuration.  Check in `secrets.py` if you have commented out the correct TTN_USER and TTN_PASSWORD for your application.

<hr>

<a name="deployment"></a>
# Deployment

<a name="newdeploy"></a>
## Deployment on a New environment (Ubuntu 18.04)

There are 3 main parts of deployment - Django, NPM and Docker.

1. Navigate to where you would like the application to sit & clone the master/development branch from room-sensors repo using ‘git clone https://gitlab.com/iotop/room-sensors.git’ or `git pull  https://gitlab.com/iotop/room-sensors.git dev` to pull from dev branch.

2. cd into the /roomsensors/webapplication folder and run ‘pip3 install -r requirements.txt’. It is important to use pip3 instead of pip as the version of Django we are using (2.0.7) conflicts with older versions of python. If you don’t have pip3 you can install it with ‘sudo apt install python3-pip’

hint: In addition to basic services such as git & vim that you would have intuitevley installed, use 'sudo apt install net-tools' so you can use commands like 'ifconfig' and 'netstat' later down the line if you need to.

<a name="Django"></a>
## Django 

1. Navigate to the room-sensors/webapplication/heatmap folder and create a file called `secrets.py` using 'sudo vim secrets.py’. This file contains variables we need kept private and an example can be found in our sensitive repo here https://gitlab.com/iotop/sensitive/-/blob/master/Roomsensors/secrets.py - (obviously you will need to be logged in to view this file). This file can be copy and pasted, ensure you have the right application uncommented.

2. Modify the IP address to match your hosts IP in  ALLOWED_HOSTS & CORS_ORIGIN_WHITELIST. Use ifconfig to find your IP.

![data](Img/whitelist.png)

3. Change line 26 in settings.py to 'DEBUG = False'.  Save file but leave this terminal running for now and start another terminal for the npm instruction.

<a name="Node"></a>
## Node

**NOTE:** Please DO NOT run sudo with npm commands inside server.

1. cd into room-sensors/webapplication/front-end & run `sudo apt install npm` to install NPM and Node.

2. Run `npm install` to install any dependencies.

3. Run `npm install moment --save` in order to use time and save it to dependency library.

4. Run `npm uninstall dotenv --save` to uninstall the development package of dotenv as it is no longer required as it is only used during development only.  Also make sure to delete/comment out `require('dotenv').config();` in `/front-end/src/App.css`.  The .env functionality is already implemented in the react build script so there is no need to add this library.

5. Please make sure there is no existed build folder inside `/frontend/`.  You can rename the existed build folder as a backup if needed.  Run `npm run-script build` – this will populate a new build folder in `room-sensors/webapplication/front-end`.

6. In Deployment server, `npm start` is no longer required as docker command will run application altogether.

<a name="Docker"></a>
## Docker 

Docker has been implemented in the development server as well as Production in order to contain each project in its own container.  If you are deploying a brand new server within your local machine/new server, please follow from step 1.  However, within Production / Development server - docker has already been implemented so you can skip docker instruction.  If you have done any changes to files within `/frontend/` or `/webapplication/`, please start from Step 7.


1. Navigate to `/webapplication/`.  Install docker and docker-compose with 'sudo apt install docker.io' 'sudo apt install docker-compose'

2. run 'sudo systemctl start docker'

3. run 'sudo systemctl enable docker'

4. run 'docker --version' and 'docker run hello-world' to verify you have installed docker correctly.

5. Copy over the .env files from the sensitive repository into the front-end directory and set where you wish to serve the application from (preferably from your own domain. This is the URL. Not the hostname/ip)

6. Before booting up the container via docker, cd into the front-end directory and run 'npm install' to install all of the dependencies then 'npm run-script build' to build the actual website.

7. Navigate to the webapplication (the folder where Dockerfile & docker-compose.yml is) and run 'sudo docker-compose build" followed by 'sudo docker-compose up'

8. Open docker-compose.yml with vim and ensure that web > volumes (line 27) matches the path of your environment e.g "/home/thanos/room-sensors/webapplication/db.sqlite3:/usr/src/app/db.sqlite3".

On the dev server, this path will need to be `/home/student/Room-Sensor/`


![data](Img/volume.png)


9. Any changes made in frontend or Django files will require you to run the following steps: 

- Go to `/frontend/` and run `npm install`, `npm run-script build`.

- Go back to folder `/webapplication/` to run `docker-compose build` to rebuild the whole container image.  

- After successfully creating a new image `docker-compose up`, this will start heatmap container on the server if there is no conflicts.  However, there is also a possibility this command may fail if there is any issues with migration files/merge conflict/static files unfound.    One of the advantages from using Docker, python3/python commands are no longer needed as docker file has explicitly specify the correct version of Python to migrate / detect any changes and migrate new changes to database without having to run `makemigrations / migrate / sqlmigrate` as in the development process.    

- Use `python3 manage.py showmigrations` to check which migration files have/not been migrated if needed.

10. There is a chance of failure during building with no space error; however, this can be fixed with `docker system prune` to delete unused / old images in the system.  To find out the errors inside container, run `docker-compose logs web` (in ~/roomsensors/webapplication). 

11. ```Error: for nginx Cannot start service nginx: driver failed programming external connectivity on endpoint... Error starting userland proxy: listen tcp 0.0.0.0:80: bind: address already in use``` 

This error is used by Apache2 service took over port 80 so `sudo systemctl stop apache2` may free the port to help running `docker-compose up` successfully.


<a name="SQLite3 in Deployment"></a>
## SQLite3 in Deployment
- After deployment, a brand new db.sqlite3 will be created in /webapplication/.  It is important to preload database with all existed devices that will send data to our server or "Sensor query not found" error will appear and data will not be saved in the database, which will fail to display on React.  Some item will be null as default and some values will need to be inserted as a dummy value, which will be updated later when live data arrives.


You can access sqlite3 by `sqlite3` in terminal.
- To open database, `.open db.sqlite3`.
- Check schema in order to do the insert statement correctly - `.schema api_sensor`  
- Currently, our schema in development server is

```
CREATE TABLE IF NOT EXISTS "api_sensor" ("serialID" integer NOT NULL PRIMARY KEY, "temperature" real NULL, "humidity" real NULL, "x" integer NOT NULL, "y" integer NOT NULL, "name" varchar(50) NOT NULL, "time" datetime NULL, "co2_ppm" integer NULL, "floor" varchar(30) NOT NULL);

```
Example to insert value, for temperature and humidity sensor. 

```
INSERT INTO api_sensor(serialID, temperature, humidity, x, y, name, time, co2_ppm, floor)
VALUES(xxx, 12.4, 45.3, 600, 80, "barry", null, null, "two");
```

Example to insert value, for co2_ppm.

```
INSERT INTO api_sensor(serialID, temperature, humidity, x, y, name, time, co2_ppm, floor)
VALUES(xxx, null, null, 600, 50, "co2_01", null, 444, "two");
```

Production Database schema is serialID, humidity,x,y,floor,name,time,co2_ppm,temperature.

More information regarding queries for sqlite3 can be found here https://www.sqlitetutorial.net/sqlite-cheat-sheet/

<hr>

<a name="olddeploy"></a>
# Deployment - Production server (Ubuntu 16.04)

**NOTE** : This section is no longer in use as we are now using 18.04 but it may be a good background to help troubleshooting for future deployment.

1. Login to the roomsensors ubuntu server (details found in Sensitive repo)
2. CD into the webaapplication folder ~/room-sensors/webapplication and pull down any changes that needs to be deployed with 'git pull'. Ensure you are pulling from the master branch
3. Run 'docker-compose build' (make sure you are in the /webaapplication folder where the dockerfile is) - this will rebuild the app, update dependencies reflected in requirements.txt & run any migrations that haven't been applied.
4. Run 'docker-compose up' to launch the app internally on 10.25.100.164. The app is yet to be deployed externally or have a DNS applied.
5. To check the app is running correctly, visit 10.25.100.164 on the local polytech network and ensure devices are receiving payloads & that changes have been applied. To simulate a payload for testing purposes follow the instructions below or in the sensitive repo.
6. Occasionally you will need to make small security changes on the server that can't be pulled from Master. To find out what information goes where visit the roomsensors folder in the sensitive repository.
7. Note that if you are making changes to .py files you will need to rebuild the npm folder with 'npm run-script build' in the /frontend folder before running docker-compose
