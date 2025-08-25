const { contextBridge, ipcRenderer } = require('electron');

// Expose protected methods that allow the renderer process to use
// the ipcRenderer without exposing the entire object
contextBridge.exposeInMainWorld('electronAPI', {
  // Add any APIs you want to expose to your Svelte app here
  getVersion: () => process.versions.electron,
  getPlatform: () => process.platform,
  
  // Game management APIs
  getGamesFromFolder: () => ipcRenderer.invoke('get-games-from-folder'),
  openExePath: (path) => ipcRenderer.invoke('open-exe-path', path),
  chooseExe: () => ipcRenderer.invoke('choose-exe'),
  runRetroarchRom: (romPath, corePath) => ipcRenderer.invoke('run-retroarch-rom', romPath, corePath),
  
  // Event listeners for games updates
  onGamesUpdated: (callback) => {
    ipcRenderer.on('games-updated', callback);
  },
  removeGamesUpdatedListener: () => {
    ipcRenderer.removeAllListeners('games-updated');
  }
    ,

  // New APIs for folder/path selection and config
  chooseGamesFolder: () => ipcRenderer.invoke('choose-games-folder'),
  chooseRetroarchPath: () => ipcRenderer.invoke('choose-retroarch-path'),
  getGamesFolderPath: () => ipcRenderer.invoke('get-games-folder-path'),
  getRetroarchPath: () => ipcRenderer.invoke('get-retroarch-path'),
  setGamesFolderPath: (path) => ipcRenderer.invoke('set-games-folder-path', path),
  setRetroarchPath: (path) => ipcRenderer.invoke('set-retroarch-path', path)
});
