#include "PalF.h"

int PalF::readPal(std::string fileName) 
{
    std::cout << "Reading palette: " << fileName << std::endl;
    
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open palette file: " << fileName << std::endl;
        return -1;
    }

    // Read color count (4 bytes, little-endian)
    uint16_t colorCount = 0;
    file.read(reinterpret_cast<char*>(&colorCount), 2);

    // Skip 2
    file.seekg(2, std::ios::cur);
    
    std::cout << "\tColor count: " << colorCount << std::endl;

    // Validate color count
    if (colorCount == 0 || colorCount > 256) {
        std::cerr << "ERROR: Invalid color count: " << colorCount << std::endl;
        file.close();
        return -2;
    }

    // Clear and prepare colors vector
    colors.clear();
    colors.reserve(256);  // Always ensure 256 colors

    // Read each color (ABGR format, 4 bytes per color)
    for (uint32_t i = 0; i < colorCount; i++) 
    {
        // uint32_t abgr;
        // palFile.read(reinterpret_cast<char*>(&abgr), 4);

        ApeColor color;
        file.read(reinterpret_cast<char*>(&color), 4);

        // Convert RGBA to BGRA if necessary
        if (colorModel == 1) {
            std::swap(color.r, color.b);
        }


        colors.push_back(color);

        // Debug output
        std::cout << "\tColor " << i << ": R=" << static_cast<int>(color.r) 
                  << " G=" << static_cast<int>(color.g) 
                  << " B=" << static_cast<int>(color.b) 
                  << " A=" << static_cast<int>(color.a) 
                //   << " (Raw ARGB: " << std::hex << color.a << color.r << color.g << color.b << std::dec << ")"
                  << std::endl;
    }

    file.close();

    // Fill remaining colors if necessary
    while (colors.size() < 256) {
        colors.push_back({0, 0, 0, 255});  // Fill with black (fully opaque)
    }

    return 1;
}

// Does a simple validation to see if file is valid APE palette
// Not a comprehensive check, just a quick validation of the first few bytes
// Loading in the palette later can return early if the rest is not valid
int PalF::validatePaletteFile(std::string fileName) 
{
    std::ifstream palette(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    bool isValid = false;
    // if (!palette.is_open()) {
    //     return 0;
    // }

    // // Get header info
    // ApeHeader hdr = getHeader(fileName);

    // // if pal name is empty, return false
    // if (hdr.palName.empty() || hdr.palNameSize == 0 || hdr.palNameSize < 0) {
    //     // if no palette exists then immediately return false
    //     return 0;
    // }

    // // Make sure file has .pal extension
    // std::string paletteName(hdr.palName.data());
    // if (paletteName.find(".pal") == std::string::npos) {
    //     isValid = 1;
    // } 
    // else {
    //     return 0;
    // }

    // // Read color count (4 bytes, little-endian)
    // uint16_t colorCount = 0;
    // palette.read(reinterpret_cast<char*>(&colorCount), 2);

    // // Skip 2
    // palette.seekg(2, std::ios::cur);
    
    // // Validate color count
    // if (colorCount < 0 || colorCount > 256) {
    //     palette.close();
    //     return isValid;
    // }

    isValid = true;
    palette.close();
    return isValid;
}
