const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const { Board, Led } = require('johnny-five');

const app = express();
const PORT = process.env.PORT || 3002;

// Middleware setup
app.use(bodyParser.json());
app.use(cors());

// Create a new board object with COM4 as the port
const board = new Board({ port: "COM5" });

// When the board is ready, setup your routes and start the server
board.on('ready', () => {
  // Route to handle data send
  app.post('/data-send', (req, res) => {
    const { name, color } = req.body;
    console.log(`Received data - Name: ${name}, Color: ${color}`);
    
    // Example: Blink an LED based on the received data
    const led = new Led(13); // Pin 13 is a common LED pin on Arduino boards
    led.blink(100); // Blink the LED every 500 milliseconds
    
    res.sendStatus(200);
  });

  // Server listening
  app.listen(PORT, () => {
    console.log(`Server listening on port ${PORT}`);
  });
});
