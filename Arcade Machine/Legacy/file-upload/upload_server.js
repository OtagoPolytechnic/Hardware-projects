const express = require('express');
const multer = require('multer');
const path = require('path');
const fs = require('fs');
const cors = require('cors');
const unzipper = require('unzipper');

const app = express();
const port = 3001; // Ensure this is different from your React app's port

// Enable CORS for all requests
app.use(cors());

// Middleware to parse form data
app.use(express.urlencoded({ extended: true }));
app.use(express.json());

// Function to check and create directory
const checkAndCreateDir = (dir) => {
  if (!fs.existsSync(dir)) {
    fs.mkdirSync(dir, { recursive: true });
  }
};

// Configure multer for file storage
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    const uploadDir = path.join(__dirname, 'uploads');
    checkAndCreateDir(uploadDir);
    cb(null, uploadDir);
  },
  filename: (req, file, cb) => {
    cb(null, file.originalname);
  }
});

const upload = multer({ storage }).fields([
  { name: 'zipFile', maxCount: 1 },
  { name: 'image', maxCount: 1 }
]); // Accept a .zip file and an image file

app.post('/upload', (req, res, next) => {
  upload(req, res, async function (err) {
    if (err instanceof multer.MulterError) {
      return res.status(500).send({ error: err.message });
    } else if (err) {
      return res.status(500).send({ error: err.message });
    }

    const { devName, gameName, isAgeRestricted, controller } = req.body; // Get form data
    const baseDir = path.join(__dirname, 'moved', isAgeRestricted); // Base directory based on ESRB rating
    const gameDir = path.join(baseDir, gameName); // Game specific directory

    // Check and create directory
    checkAndCreateDir(baseDir);
    checkAndCreateDir(gameDir);

    const zipFilePath = path.join(__dirname, 'uploads', req.files.zipFile[0].filename);
    const imageFilePath = path.join(__dirname, 'uploads', req.files.image[0].filename);
    const textFilePath = path.join(gameDir, 'details.txt');

    // Create the .txt file with details
    const textContent = `Dev Name: ${devName}\nGame Name: ${gameName}\nESRB Rating: ${isAgeRestricted}\nController: ${controller}`;
    fs.writeFileSync(textFilePath, textContent);

    // Extract the zip file
    fs.createReadStream(zipFilePath)
      .pipe(unzipper.Extract({ path: gameDir }))
      .on('close', () => {
        // Move the image file to the game directory
        const destImagePath = path.join(gameDir, req.files.image[0].originalname);
        fs.renameSync(imageFilePath, destImagePath);

        // Delete the uploaded zip file
        fs.unlinkSync(zipFilePath);
        res.send('Files uploaded and extracted successfully');
      })
      .on('error', (err) => {
        res.status(500).send({ error: err.message });
      });
  });
});

// Start the server
app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});
