const retroarch = "C:\\RetroArch-Win64\\";
const retroarchExe = `${retroarch}retroarch.exe`;
const retroarchCore =  `${retroarch}cores`;


const coreMap = { // this is the supperted roms
    smc: `${retroarchCore}\\snes9x2002_libretro.dll`,
};

const supportedImageTypes = ['.jpg', '.jpeg', '.png', '.gif', '.bmp', '.webp'];
const supportedExts = ['.exe', '.lnk',];

// Push all coreMap keys to supportedExts
Object.keys(coreMap).forEach(key => supportedExts.push(`.${key}`));

module.exports = {
    retroarchExe,
    coreMap,
    supportedImageTypes,
    supportedExts
};


