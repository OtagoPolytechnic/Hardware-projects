const { app, BrowserWindow, ipcMain, dialog, shell } = require('electron');
const path = require('path');
const fs = require('fs').promises;
const os = require('os');
const isDev = process.env.NODE_ENV === 'development';
const { spawn } = require('child_process');
const fsSync = require('fs');

// Import paths and supported types
const { retroarchExe, supportedImageTypes, supportedExts, getRetroArchPaths } = require('./coremap.cjs');

// Constants
const PATHS = {
  desktop: path.join(os.homedir(), 'Desktop'),
  get games() { return path.join(this.desktop, 'Games') },
  preload: path.join(__dirname, 'preload.cjs'),
  icon: path.join(__dirname, '../public/favicon.ico'),
  distHtml: path.join(__dirname, '../dist/index.html')
};

/**
 * Configuration manager for arcade settings
 */
class ConfigManager {
  constructor() {
    this.configPath = path.join(app.getPath('userData'), 'arcade-config.json');
  }

  /**
   * Read configuration from file
   * @returns {Object} Configuration object
   */
  read() {
    try {
      if (fsSync.existsSync(this.configPath)) {
        const raw = fsSync.readFileSync(this.configPath, 'utf-8');
        return JSON.parse(raw);
      }
    } catch (error) {
      console.error('Failed to read config:', error);
    }
    return {};
  }

  /**
   * Write configuration to file
   * @param {Object} config - Configuration object to save
   */
  write(config) {
    try {
      fsSync.writeFileSync(this.configPath, JSON.stringify(config, null, 2));
    } catch (error) {
      console.error('Failed to write config:', error);
    }
  }

  /**
   * Update specific configuration value
   * @param {string} key - Configuration key
   * @param {*} value - Configuration value
   */
  update(key, value) {
    const config = this.read();
    config[key] = value;
    this.write(config);
    return config[key];
  }

  /**
   * Get configuration value with fallback
   * @param {string} key - Configuration key
   * @param {*} fallback - Fallback value if key doesn't exist
   * @returns {*} Configuration value or fallback
   */
  get(key, fallback) {
    const config = this.read();
    return config[key] || fallback;
  }
}

/**
 * Path manager for RetroArch and games
 */
class PathManager {
  constructor(configManager) {
    this.config = configManager;
  }

  /**
   * Get games folder path
   * @returns {string} Games folder path
   */
  getGamesFolderPath() {
    return this.config.get('gamesFolder', PATHS.games);
  }

  /**
   * Get RetroArch base directory
   * @returns {string} RetroArch base directory
   */
  getRetroarchBaseDir() {
    return this.config.get('retroarchFolder', path.dirname(retroarchExe));
  }

  /**
   * Get RetroArch executable path
   * @returns {string} RetroArch executable path
   */
  getRetroarchPath() {
    return path.join(this.getRetroarchBaseDir(), 'retroarch.exe');
  }

  /**
   * Get RetroArch cores directory
   * @returns {string} RetroArch cores directory path
   */
  getRetroarchCoreDir() {
    return path.join(this.getRetroarchBaseDir(), 'cores');
  }

  /**
   * Get supported ROMs configuration
   * @returns {Object} Supported ROMs object
   */
  getSupportedRoms() {
    const { supportedRoms } = getRetroArchPaths(this.getRetroarchBaseDir());
    return supportedRoms;
  }
}

// Initialize managers
const configManager = new ConfigManager();
const pathManager = new PathManager(configManager);

// Window configuration
const WINDOW_CONFIG = {
  webPreferences: {
    nodeIntegration: false,
    contextIsolation: true, 
    enableRemoteModule: false,
    webSecurity: true,
    preload: PATHS.preload
  },
  icon: PATHS.icon
};

/**
 * Game management utilities
 */
class GameManager {
  /**
   * Scan games folder for supported game files
   * @param {string} folderPath - Optional folder path to scan
   * @returns {Promise<Array>} Array of game objects
   */
  static async scanGamesFolder(folderPath) {
    folderPath = folderPath || pathManager.getGamesFolderPath();
    
    // Ensure the folder exists, create if not
    try {
      await fs.access(folderPath);
    } catch {
      await fs.mkdir(folderPath, { recursive: true });
      return [];
    }

    try {
      const files = await fs.readdir(folderPath);
      const gameFiles = files.filter(file => {
        const ext = path.extname(file).toLowerCase();
        return supportedExts.includes(ext);
      });

      return await Promise.all(
        gameFiles.map(async file => {
          const filePath = path.join(folderPath, file);
          const baseName = path.basename(file, path.extname(file));
          const image = await this.findMatchingImage(folderPath, baseName);

          return {
            name: baseName,
            path: filePath,
            image
          };
        })
      );
    } catch (error) {
      console.error('Error scanning games folder:', error);
      return [];
    }
  }

  /**
   * Find matching image for a game
   * @param {string} folderPath - Folder to search in
   * @param {string} baseName - Base name of the game file
   * @returns {Promise<string|null>} Image path or null if not found
   */
  static async findMatchingImage(folderPath, baseName) {
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

  /**
   * Open game file using the system default application
   * @param {string} gamePath - Path to the game file
   * @returns {Promise<boolean>} Success status
   */
  static async openGame(gamePath) {
    try {
      const ext = path.extname(gamePath).toLowerCase();
      
      if (supportedExts.includes(ext)) {
        await shell.openPath(gamePath);
        return true;
      } else {
        console.error('Unsupported file type:', ext);
        return false;
      }
    } catch (error) {
      console.error('Error opening game:', error);
      return false;
    }
  }

  /**
   * Run ROM using RetroArch
   * @param {string} romPath - Path to the ROM file
   * @param {string} coreFileName - RetroArch core filename
   * @returns {Promise<boolean>} Success status
   */
  static async runRetroarchRom(romPath, coreFileName) {
    const retroarchExePath = pathManager.getRetroarchPath();
    
    if (!fsSync.existsSync(retroarchExePath)) {
      console.error(`RetroArch executable not found at: ${retroarchExePath}`);
      return false;
    }
    
    try {
      const coreDir = pathManager.getRetroarchCoreDir();
      const fullCorePath = path.join(coreDir, coreFileName);
      
      spawn(retroarchExePath, ['-L', fullCorePath, romPath], { 
        detached: false, 
        stdio: 'ignore' 
      });
      
      console.log(`RetroArch launched with core: ${fullCorePath} and ROM: ${romPath}`);
      return true;
    } catch (error) {
      console.error('Error launching RetroArch:', error);
      return false;
    }
  }
}

/**
 * Dialog utilities for folder selection
 */
class DialogManager {
  /**
   * Show folder selection dialog and update config
   * @param {string} configKey - Configuration key to update
   * @param {string} pathSuffix - Optional path suffix to append
   * @returns {Promise<string|null>} Selected folder path or null
   */
  static async selectFolder(configKey, pathSuffix = '') {
    try {
      const result = await dialog.showOpenDialog({ properties: ['openDirectory'] });
      
      if (result.canceled || !result.filePaths[0]) {
        return null;
      }
      
      const selectedPath = result.filePaths[0];
      configManager.update(configKey, selectedPath);
      
      return pathSuffix ? path.join(selectedPath, pathSuffix) : selectedPath;
    } catch (error) {
      console.error(`Error selecting folder for ${configKey}:`, error);
      return null;
    }
  }
}

/**
 * Create main application window
 */
function createWindow() {
  const mainWindow = new BrowserWindow(WINDOW_CONFIG);

  if (isDev) {
    mainWindow.loadURL('http://localhost:5173');
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.loadFile(PATHS.distHtml);
  }
}

/**
 * Application lifecycle events
 */
app.whenReady().then(() => {
  createWindow();
});

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

/**
 * IPC
 * Communicate asynchronously
 * from the main process to renderer processes
 *  https://www.electronjs.org/docs/latest/tutorial/ipc
 *  https://www.electronjs.org/docs/latest/api/ipc-main/ 
 */

/**
 * IPC Handlers - Game Management
 */
ipcMain.handle('get-games-from-folder', async (event, folderPath) => {
  try {
    return await GameManager.scanGamesFolder(folderPath || pathManager.getGamesFolderPath());
  } catch (error) {
    console.error('Error getting games from folder:', error);
    return [];
  }
});

ipcMain.handle('open-exe-path', async (event, gamePath) => {
  return await GameManager.openGame(gamePath);
});

ipcMain.handle('run-retroarch-rom', async (event, romPath, coreFileName) => {
  return await GameManager.runRetroarchRom(romPath, coreFileName);
});



/**
 * IPC Handlers - Configuration Management
 */
ipcMain.handle('choose-games-folder', async () => {
  return await DialogManager.selectFolder('gamesFolder');
});

ipcMain.handle('choose-retroarch-path', async () => {
  return await DialogManager.selectFolder('retroarchFolder');
});

ipcMain.handle('get-games-folder-path', () => {
  return pathManager.getGamesFolderPath();
});

ipcMain.handle('get-retroarch-path', () => {
  return configManager.get('retroarchFolder', path.dirname(retroarchExe));
});


ipcMain.handle('set-games-folder-path', (event, folderPath) => {
  return configManager.update('gamesFolder', folderPath);
});

ipcMain.handle('set-retroarch-path', (event, folderPath) => {
  configManager.update('retroarchFolder', folderPath);
  return folderPath;
});

ipcMain.handle('get-supported-roms', () => {
  return pathManager.getSupportedRoms();
});
