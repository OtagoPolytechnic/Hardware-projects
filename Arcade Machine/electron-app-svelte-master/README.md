# Retro Arcade Collection
built in 2025

Retro Arcade Collection is an Electron-based application built with Svelte 4 and Vite. It allows users to manage and launch games from a local folder.

## Features

- Browse and launch games from a local folder.

- Integrates with RetroArch to run supported ROM files.
- Automatically detects `ROMS`, `.exe` and `.lnk` (Windows shortcuts) files in the `Desktop/game` folder.
- Displays custom images for games if available. These images should be placed in the `Desktop/game` folder with the same name as the game executable.
- Support for Windows

## supported file types
- `.exe` files
- `.lnk` files (Windows shortcuts)
- RetroArch ROM files (see supported formats below)

### Supported RetroArch ROM formats:
-  `smc` files (SNES ROMs)



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

4. build app:
   ```bash
   npm run electron-pack
   ```


## Usage Instructions


1)	Make a folder on the desktop called “Games”, if there is not folder when you open the program it will make the folder
   <img width="97" height="96" alt="image" src="https://github.com/user-attachments/assets/bf456543-ed0b-470e-a43c-3b17f6d86a3c" />
   
2)	Put in shortcuts to the game you want to add inside the “Games” folder. 
3)	Add PNG or JPG with the same name as the game you added to be used as cover art (EG: street-fighter.lnk and street-fighter.png)
   <img width="855" height="385" alt="image" src="https://github.com/user-attachments/assets/acc2235c-2460-4f7d-814b-db964914fa90" />

4)	Open aracde softwhere 
<img width="1197" height="784" alt="v2" src="https://github.com/user-attachments/assets/19b1c37d-b974-4d31-ada6-a8bf61238c39" />




## Arcade set up once built 
1)	Make a folder on the desktop called “Games”. (if you don't make it the app will make it when opened)
   <img width="98" height="116" alt="image" src="https://github.com/user-attachments/assets/e4cd6af2-11e9-4d07-b548-1af60951cb2d" />
   
2)	Put the files to the Games you want to add inside the “Games” folder. this must be done before you open the app or games wont be shown
   <img width="668" height="374" alt="image" src="https://github.com/user-attachments/assets/88d4003d-38c3-4258-9a27-04390c51689c" />

3)	Add PNG or JPG with the same name as the game you added to be used as cover art
4)	Open the arcade app






