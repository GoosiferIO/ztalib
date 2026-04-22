#include "PalF.h"

/* PalF.cpp -- parser for zt1 palette files

    ztalib - ZT1 graphics parser
    https://goosifer.io/
    Copyright 2025 Eric Galvan (GoosiferIO)

    OFFICIAL REPOSITORY

        https://github.com/GoosiferIO/ztalib

    LICENSE

        MIT License (see LICENSE at end of file or at project root)

    ACKNOWLEDGMENTS

        Thank you to Jeff Bostoen for his documentation on the ZT1 file formats:
        https://github.com/jbostoen/ZTStudio/wiki/ZT1-Graphics-Explained
*/

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

/*
    MIT License

    Copyright (c) 2025 Eric Galvan (GoosiferIO)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/