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
1)	Make a folder on the desktop called “Games”
2)	Put in shortcuts to the Games you want to add inside the “Games” folder
3)	Add PNG or JPG with the same name as the game you added to be used as cover art
4)	Open exe



