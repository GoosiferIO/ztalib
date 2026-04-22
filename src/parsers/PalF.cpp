#include "PalF.h"

PalF::PalF()
{
    file = std::ifstream();
    file.exceptions(static_cast<std::ios_base::iostate>(
        std::ifstream::failbit | std::ifstream::badbit));
    colors = std::vector<ZtaColor>();
    nameSize = 0;
    name = std::vector<char>();
    location = "";
    colorModel = 0;
    numColors = 0;
}

PalF::~PalF()
{
    if (file.is_open())
    {
        file.close();
    }
    colors.clear();
    name.clear();
}

int PalF::read(std::string fileName)
{
    file.open(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open palette file: " << fileName << std::endl;
        return -1;
    }

    // Read color count (4 bytes, little-endian)
    uint16_t colorCount = 0;
    file.read(reinterpret_cast<char *>(&colorCount), 2);

    // Skip 2
    file.seekg(2, std::ios::cur);

    // Validate color count
    if (colorCount == 0 || colorCount > 256)
    {
        std::cerr << "ERROR: Invalid color count: " << colorCount << std::endl;
        file.close();
        return -2;
    }

    // Clear and prepare colors vector
    colors.clear();
    colors.reserve(256); // Always ensure 256 colors

    // Read each color (ABGR format, 4 bytes per color)
    for (uint32_t i = 0; i < colorCount; i++)
    {
        // uint32_t abgr;
        // palFile.read(reinterpret_cast<char*>(&abgr), 4);

        ZtaColor color;
        file.read(reinterpret_cast<char *>(&color), 4);

        // Convert RGBA to BGRA if necessary
        if (colorModel == 1)
        {
            std::swap(color.r, color.b);
        }

        colors.push_back(color);
    }

    file.close();

    // Fill remaining colors if necessary
    while (colors.size() < 256)
    {
        colors.push_back({0, 0, 0, 255}); // Fill with black (fully opaque)
    }

    numColors = static_cast<int>(colors.size());
    return 1;
}

ZtaColor PalF::getColor(int index)
{
    if (index < 0 || index >= numColors)
    {
        std::cerr << "ERROR: Color index out of bounds: " << index << std::endl;
        return {0, 0, 0, 255}; // Return black (fully opaque) as default
    }
    return colors[index];
}