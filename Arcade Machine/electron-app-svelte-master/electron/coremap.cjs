// Default RetroArch paths - will be overridden by user selection
const defaultRetroarch = "C:\\RetroArch-Win64\\";

// Function to get RetroArch paths based on a base directory
function getRetroArchPaths(retroarchBaseDir = defaultRetroarch) {

    const retroarchExe = `${retroarchBaseDir}\\retroarch.exe`;
    const retroarchCore = `${retroarchBaseDir}\\cores\\`;
    
    const supportedRoms = { // this is the supported roms - just filenames
        smc: `snes9x2002_libretro.dll`,

    };
    
    return {
        retroarchExe,
        retroarchCore,
        supportedRoms
    };
}

// Export default paths 
const { retroarchExe, retroarchCore, supportedRoms } = getRetroArchPaths();

const supportedImageTypes = ['.jpg', '.jpeg', '.png', '.gif', '.bmp', '.webp'];
const supportedExts = ['.exe', '.lnk', ];

// Push all supportedRoms keys to supportedExts
Object.keys(supportedRoms).forEach(key => supportedExts.push(`.${key}`));

module.exports = {
    retroarchExe,
    supportedRoms,
    supportedImageTypes,
    supportedExts,
    getRetroArchPaths
};


