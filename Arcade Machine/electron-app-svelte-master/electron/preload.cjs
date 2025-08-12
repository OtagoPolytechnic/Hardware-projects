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
  
  // Event listeners for games updates
  onGamesUpdated: (callback) => {
    ipcRenderer.on('games-updated', callback);
  },
  removeGamesUpdatedListener: () => {
    ipcRenderer.removeAllListeners('games-updated');
  }
});
