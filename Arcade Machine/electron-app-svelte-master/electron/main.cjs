const { app, BrowserWindow, ipcMain, dialog, shell } = require('electron');
const path = require('path');
const fs = require('fs').promises;
const os = require('os');
const isDev = process.env.NODE_ENV === 'development';

// Set your RetroArch executable path here
const retroarchExe = 'C:\\\RetroArch-Win64\\retroarch.exe';


function createWindow() {
  // Create the browser window
  const mainWindow = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      enableRemoteModule: false,
      webSecurity: false, // Allow loading local files
      preload: path.join(__dirname, 'preload.cjs')
    },
    icon: path.join(__dirname, '../public/favicon.ico')
  });

  // Load the app
  if (isDev) {
    mainWindow.loadURL('http://localhost:5173');
    // Open DevTools in development
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.loadFile(path.join(__dirname, '../dist/index.html'));
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

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

// Security: Prevent new window creation
app.on('web-contents-created', (event, contents) => {
  contents.on('new-window', (event, navigationUrl) => {
    event.preventDefault();
  });
});


// IPC Handlers for game management
ipcMain.handle('get-games-from-folder', async () => {
  try {
    // Look for games folder on desktop
    const desktopPath = path.join(os.homedir(), 'Desktop');
    const gamesFolder = path.join(desktopPath, 'Games');
    
    console.log('Looking for games in:', gamesFolder);
    
    // Check if games folder exists
    try {
      await fs.access(gamesFolder);
    } catch (error) {
      console.log('Games folder not found, creating it...');
      await fs.mkdir(gamesFolder, { recursive: true });
      return [];
    }
    
    const files = await fs.readdir(gamesFolder);
    const games = [];
    const supportedImageTypes = ['.jpg', '.jpeg', '.png', '.gif', '.bmp', '.webp'];
    
    // Find all executable files, shortcuts, and zip files
    const gameFiles = files.filter(file => {
      const ext = path.extname(file).toLowerCase();
      return ext === '.exe' || ext === '.lnk' || ext === '.zip';
    });
    
    for (const file of gameFiles) {
      const filePath = path.join(gamesFolder, file);
      const baseName = path.basename(file, path.extname(file));

      // Look for matching image file
      let imageUrl = null;
      for (const imageExt of supportedImageTypes) {
        const potentialImagePath = path.join(gamesFolder, baseName + imageExt);
        try {
          await fs.access(potentialImagePath);
          imageUrl = potentialImagePath; // Use original path
          break;
        } catch (error) {
          // Image not found, continue
        }
      }

      games.push({
        name: baseName,
        path: filePath,
        image: imageUrl
      });
    }

    console.log('Found games:', games);
    return games;
  } catch (error) {
    console.error('Error reading games folder:', error);
    return [];
  }
});

// Handle opening game files
ipcMain.handle('open-exe-path', async (event, gamePath) => {
  try {
    console.log('Opening game:', gamePath);
    const ext = path.extname(gamePath).toLowerCase();
    
    if (ext === '.lnk') {
      // For shortcuts, use shell.openPath which handles .lnk files
      await shell.openPath(gamePath);
    } else if (ext === '.exe') {
      // For executables, use shell.openPath as well
      await shell.openPath(gamePath);
    } else {
      console.error('Unsupported file type:', ext);
    }
  } catch (error) {
    console.error('Error opening game:', error);
  }
});


// Handle choosing an executable file
ipcMain.handle('choose-exe', async () => {
  try {
    const result = await dialog.showOpenDialog({
      properties: ['openFile'],
      filters: [
        { name: 'Executable Files', extensions: ['exe'] },
        { name: 'Shortcut Files', extensions: ['lnk'] },
        { name: 'All Supported', extensions: ['exe', 'lnk'] }
      ]
    });
    
    if (!result.canceled && result.filePaths.length > 0) {
      const sourcePath = result.filePaths[0];
      const fileName = path.basename(sourcePath);
      const desktopPath = path.join(os.homedir(), 'Desktop');
      const gamesFolder = path.join(desktopPath, 'game');
      const destPath = path.join(gamesFolder, fileName);
      
      // Ensure games folder exists
      await fs.mkdir(gamesFolder, { recursive: true });
      
      // Copy the file to games folder
      await fs.copyFile(sourcePath, destPath);
      
      console.log('Game added:', destPath);
      
      // Notify renderer about games update
      const mainWindow = BrowserWindow.getAllWindows()[0];
      if (mainWindow) {
        const games = await getGamesFromFolder();
        mainWindow.webContents.send('games-updated', games);
      }
      
      return destPath;
    }
  } catch (error) {
    console.error('Error choosing game file:', error);
  }
});

// Helper function to get games (reused in choose-exe handler)
async function getGamesFromFolder() {
  try {
    const desktopPath = path.join(os.homedir(), 'Desktop');
    const gamesFolder = path.join(desktopPath, 'game');
    
    try {
      await fs.access(gamesFolder);
    } catch (error) {
      await fs.mkdir(gamesFolder, { recursive: true });
      return [];
    }
    
    const files = await fs.readdir(gamesFolder);
    const games = [];
    const supportedImageTypes = ['.jpg', '.jpeg', '.png', '.gif', '.bmp', '.webp'];
    
    // Find all executable files, shortcuts, and zip files
    const gameFiles = files.filter(file => {
      const ext = path.extname(file).toLowerCase();
      return ext === '.exe' || ext === '.lnk' || ext === '.zip';
    });
    
    for (const file of gameFiles) {
      const filePath = path.join(gamesFolder, file);
      const baseName = path.basename(file, path.extname(file));
      
      let imageUrl = null;
      for (const imageExt of supportedImageTypes) {
        const potentialImagePath = path.join(gamesFolder, baseName + imageExt);
        try {
          await fs.access(potentialImagePath);
          imageUrl = potentialImagePath; // Use original path
          break;
        } catch (error) {
          // Image not found, continue
        }
      }
      
      games.push({
        name: baseName,
        path: filePath,
        image: imageUrl
      });
    }
    
    return games;
  } catch (error) {
    console.error('Error reading games folder:', error);
    return [];
  }
}

// Add IPC handler for running PowerShell script
ipcMain.handle('run-retroarch-rom', async (event, romPath, corePath) => {
  const { spawn } = require('child_process');
  const fsSync = require('fs');

  // Check if RetroArch exists
  if (!fsSync.existsSync(retroarchExe)) {
    console.error(`RetroArch executable not found at: ${retroarchExe}`);
    return false;
  }
  const args = ['-L', corePath, romPath];
  spawn(retroarchExe, args, { detached: true, stdio: 'ignore' });
  console.log(`RetroArch launched with core: ${corePath} and ROM: ${romPath}`);
  return true;
});
