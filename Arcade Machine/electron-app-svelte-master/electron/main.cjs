const { app, BrowserWindow, ipcMain, dialog, shell } = require('electron');
const path = require('path');
const fs = require('fs').promises;
const os = require('os');
const isDev = process.env.NODE_ENV === 'development';
const { spawn } = require('child_process');
const fsSync = require('fs');

// Import paths and supported types
const { retroarchExe, supportedImageTypes, supportedExts } = require('./coremap.cjs');

// Constants
const PATHS = {
  desktop: path.join(os.homedir(), 'Desktop'),
  get games() { return path.join(this.desktop, 'Games') },
  preload: path.join(__dirname, 'preload.cjs'),
  icon: path.join(__dirname, '../public/favicon.ico'),
  distHtml: path.join(__dirname, '../dist/index.html')
};

// Window configuration
const WINDOW_CONFIG = {
  width: 1200,
  height: 800,
  webPreferences: {
    nodeIntegration: false,
    contextIsolation: true,
    enableRemoteModule: false,
    webSecurity: false,
    preload: PATHS.preload
  },
  icon: PATHS.icon
};

// Error handler
const handleError = (operation, error) => {
  console.error(`Error during ${operation}:`, error);
  return [];
};

// Game scanning function
async function scanGamesFolder(folderPath) {
  // Ensure the folder exists, create if not
  try {
    await fs.access(folderPath);
  } catch {
    await fs.mkdir(folderPath, { recursive: true });
    return [];
  }

  // Read all files in the folder
  const files = await fs.readdir(folderPath);

  // Only keep files with supported extensions
  const gameFiles = files.filter(file => {
    const ext = path.extname(file).toLowerCase();
    return supportedExts.includes(ext);
  });

  // Map each game file to an object with name, path, and image
  const games = await Promise.all(
    gameFiles.map(async file => {
      const filePath = path.join(folderPath, file);
      const baseName = path.basename(file, path.extname(file));
      const image = await findMatchingImage(folderPath, baseName);

      return {
        name: baseName,
        path: filePath,
        image
      };
    })
  );

  return games;
}

// Helper function to find matching image
async function findMatchingImage(folderPath, baseName) {
  for (const imageExt of supportedImageTypes) {
    const potentialImagePath = path.join(folderPath, baseName + imageExt);
    try {
      await fs.access(potentialImagePath);
      return potentialImagePath;
    } catch {
      continue;
    }
  }
  return null;
}

function createWindow() {
  const mainWindow = new BrowserWindow(WINDOW_CONFIG);

  if (isDev) {
    mainWindow.loadURL('http://localhost:5173');
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.loadFile(PATHS.distHtml);
  }
}

// This method will be called when Electron has finished initialization
app.whenReady().then(() => {
  createWindow();
});

// Quit when all windows are closed, except on macOS
app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});


// Security: Prevent new window creation
app.on('web-contents-created', (event, contents) => {
  contents.on('new-window', (event, navigationUrl) => {
    event.preventDefault();
  });
});


// IPC Handlers for game management
ipcMain.handle('get-games-from-folder', () => 
  scanGamesFolder(PATHS.games).catch(err => handleError('scanning games folder', err))
);

// Handle opening game files
ipcMain.handle('open-exe-path', async (event, gamePath) => {
  try {
    const ext = path.extname(gamePath).toLowerCase();

    if (supportedExts.includes(ext)) {
      await shell.openPath(gamePath);
    } else {
      console.error('Unsupported file type:', ext);
    }
  } catch (error) {
    handleError('opening game', error);
  }
});


// run roms via retroarch
ipcMain.handle('run-retroarch-rom', async (event, romPath, corePath) => {

  const retroarchExist = fsSync.existsSync(retroarchExe);
  if (!retroarchExist) {
    console.error(`RetroArch executable not found at: ${retroarchExe}`);
    return false;
  }

  // Launch RetroArch with the specified core and ROM 
  //EG: (C:\RetroArch-Win64\retroarch.exe -L <corePath> <romPath>)
  spawn(retroarchExe, ['-L', corePath, romPath], { detached: false, stdio: 'ignore' });
  console.log(`RetroArch launched with core: ${corePath} and ROM: ${romPath}`);
  return true;
});
