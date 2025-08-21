# Retro Arcade Collection
built in 2025

Retro Arcade Collection is an Electron-based application built with Svelte 4 and Vite. It allows users to manage and launch games from a local folder.

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
1)	Make a folder on the desktop called “Games”. (if you don't make it the app will make it when opened)
   <img width="98" height="116" alt="image" src="https://github.com/user-attachments/assets/e4cd6af2-11e9-4d07-b548-1af60951cb2d" />
   
2)	Put in shortcuts to the Games you want to add inside the “Games” folder. this must be done before you open the app or games wont be shown
   <img width="668" height="374" alt="image" src="https://github.com/user-attachments/assets/88d4003d-38c3-4258-9a27-04390c51689c" />

3)	Add PNG or JPG with the same name as the game you added to be used as cover art
4)	Open exe






