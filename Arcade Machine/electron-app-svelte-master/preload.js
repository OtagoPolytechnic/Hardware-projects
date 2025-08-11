const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electronAPI', {
    chooseExe: () => ipcRenderer.invoke('choose-exe'),
    openExePath: (path) => ipcRenderer.send('open-exe-path', path)
});
