const retroarch = "C:\\RetroArch-Win64\\";
const retroarchExe = `${retroarch}retroarch.exe`;
const retroarchCore =  `${retroarch}cores\\`;

const supportedRoms = { // this is the supported roms
    smc: `${retroarchCore}snes9x2002_libretro.dll`,
};

const supportedImageTypes = ['.jpg', '.jpeg', '.png', '.gif', '.bmp', '.webp'];
const supportedExts = ['.exe', '.lnk',];

// Push all supportedRoms keys to supportedExts
Object.keys(supportedRoms).forEach(key => supportedExts.push(`.${key}`));

module.exports = {
    retroarchExe,
    supportedRoms,
    supportedImageTypes,
    supportedExts
};


