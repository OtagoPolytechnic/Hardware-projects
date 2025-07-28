# Quick Start
This file is simply made to help get the project up and running with as minimal reading as possible. You should consult the full [README.md](https://gitlab.com/iotop/room-sensors/-/blob/master/webapplication/README.md) if you need any more details or are encountering some issues.

## Table of Contents
- [Production Deployment](#prod)
    * [Prepartion](#prod-prep)
    * [Deployment](#prod-deploy)
    * [Common Issues/FAQ](#prod-faq)
- [Development Deployment](#dev)
    * [Preparation](#dev-prep)
    * [Deployment](#dev-deploy)

<a name="prod"></a>
## Production Deployment
This section is for deploying this software on a live production server meant to serve the public. This guide will assume you're running Ubuntu 20.04 LTS, have docker and docker-compose already installed, have your integrations set up with the Things network and the relevant ports have been open to serve traffic (ports 80/443).

<a name="prod-prep"></a>
### Preparation
First thing you'll need to do is clone the repository.

`git clone https://gitlab.com/iotop/room-sensors`

After that, you'll need to install Node.js.

`sudo apt install npm` (Ubuntu/Debian)

`sudo pacman -Syu npm` (Arch)

Lastly, you'll want to configure the following files to your needs. These files should be explain themselves what needs to be edited.
- ./heatmap/secrets.py (TTN details and Things network secret key)
- ./heatmap/.env (Database details and same URLs in the front-end)
- ./front-end/.env (The application's URLs)

I highly recommend symbolic linking your docker directory to somewhere more convient using the following command.

`ln -s <parentdir>/room-sensors/webapplication <targetdir/name>`

<a name="prod-deploy"></a>
### Deployment
Next we need to build the front end. With Node installed, we got into the front-end directory and run the following commands.

`npm install` (Install all dependencies)

`npm run-script build` (Builds the front end. This can take a moment)

Note that you will **HAVE** to do this before anytime you want to rebuild the docker container.

After that has been built, go back into the application's directory and build then start it. Should work like any other container.

`docker-compose build` `docker-compose up -d` **OR** `docker-compose build && docker-compose up -d`

Now if everything went well, you'll simply need to configure your reverse proxy to serve the website.

<a name="prod-faq"></a>
### Common Issues/FAQ
> Ports are in use

Please check if you have apache or nginx already running on your system. It might have come pre-installed with your distro like Ubuntu can.

`sudo systemctl disable nginx.service` and `sudo systemctl disable apache.service` to disable both.

> How do I access the database?

Currently the database is using sqlite3 so you can simply use `sqlite3 db.sqlite3` to access it. There is also sqlitebrowser.

> TTN is having issues.

Chances are all of your TTN issues are going to be because secrets.py is misconfigured or you haven't used the right details from the Things network. Please double check them.


All of your other issues will more than likely be in the readme.