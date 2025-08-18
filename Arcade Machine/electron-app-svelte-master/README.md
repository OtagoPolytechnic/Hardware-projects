# Retro Arcade Collection

Retro Arcade Collection is an Electron-based application built with Svelte and Vite. It allows users to manage and launch games from a local folder.

## Features

- Browse and launch games from a local folder.
- Automatically detects `.exe` and `.lnk` (Windows shortcuts) files in the `Desktop/game` folder.
- Displays custom images for games if available. These images should be placed in the `Desktop/game` folder with the same name as the game executable.
- Cross-platform support for Windows, macOS, and Linux.

---


## Setup Instructions

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd electron-app-svelte-master
   ```
2. Install dependencies:
   ```bash
   npm install
   ```
3. Start the development server:
   ```bash
   npm run dev
   ```
4. Build and Run the Electron application:
   ```bash
   npm run electron
   ```


## build exe Instructions

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd electron-app-svelte-master
   ```
2. Install dependencies:
   ```bash
   npm install
   ```

3. build app:
   ```bash
   npm run electron-pack
   ```

## Arcade set up for end user
1)	Make a folder on the desktop called “Games”, if there is not folder when you open the program it will make the folder
   <img width="97" height="96" alt="image" src="https://github.com/user-attachments/assets/bf456543-ed0b-470e-a43c-3b17f6d86a3c" />
   
2)	Put in shortcuts to the game you want to add inside the “Games” folder. 
3)	Add PNG or JPG with the same name as the game you added to be used as cover art (EG: street-fighter.lnk and street-fighter.png)
   <img width="855" height="385" alt="image" src="https://github.com/user-attachments/assets/acc2235c-2460-4f7d-814b-db964914fa90" />

4)	Open aracde softwhere 
<img width="1197" height="784" alt="v2" src="https://github.com/user-attachments/assets/19b1c37d-b974-4d31-ada6-a8bf61238c39" />


