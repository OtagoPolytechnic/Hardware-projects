const { SerialPort } = require('serialport');
const express = require("express");
const bodyParser = require("body-parser");
const cors = require("cors");

const app = express();
const PORT = process.env.PORT || 3002;

// Middleware setup
app.use(bodyParser.json());
app.use(cors());

let portser; // Declare portser variable outside

SerialPort.list().then(ports => {
  ports.forEach(function(port) {
    if(port.vendorId == "2341") {
      const COM_holder = port.path;
      console.log(COM_holder); //COM4
      // Initialize serial port 
      portser = new SerialPort({ path:COM_holder, baudRate: 115200 });
      // Open errors will be emitted as an error event
      portser.on('error', function (err) {
        console.log('Error: ', err.message);
      });
    }
  });
});

// POST route to receive data and send it through the serial port
app.post("/data-send", async (req, res) => {
  const { name, color } = req.body;
  const message = `Name: ${name}, Color: ${color}\n`; // Format the message
  console.log(message);
  const color_message = `C${color}`;
  const name_message = `T${name}`;

  if (!portser) {
    return res.status(500).send("Serial port not initialized.");
  }

  portser.write(color_message, (err) => {
    if (err) {
      return console.log('Error on write: ', err.message);
    }
    console.log('Color Message written:', color_message);
  });
  
  await sleep(1000);

  portser.write(name_message, (err) => {
    if (err) {
      return console.log('Error on write: ', err.message);
    }
    console.log('Name Message written:', name_message);
    res.send("Data sent over serial port.");
  });
});

function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}

// Server listening
app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});
