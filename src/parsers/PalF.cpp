#include "ztalib/PalF.h"

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
    m_colors = std::vector<ZtaColor>();
    m_nameSize = 0;
    m_name = std::vector<char>();
    m_location = "";
    m_colorModel = 0;
    m_numColors = 0;
}
 
PalF::~PalF()
{
    m_colors.clear();
    m_name.clear();
}

int PalF::load(std::string fileName)
{
    std::ifstream file(fileName, std::ios::binary);
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
    m_colors.clear();
    m_colors.reserve(256); // Always ensure 256 colors

    // Read each color (ABGR format, 4 bytes per color)
    for (uint32_t i = 0; i < colorCount; i++)
    {
        // uint32_t abgr;
        // palFile.read(reinterpret_cast<char*>(&abgr), 4);

        ZtaColor color;
        file.read(reinterpret_cast<char *>(&color), 4);

        // Convert RGBA to BGRA if necessary
        if (m_colorModel == 1)
        {
            std::swap(color.r, color.b);
        }

        m_colors.push_back(color);
    }

    file.close();

    // Fill remaining colors if necessary
    while (m_colors.size() < 256)
    {
        m_colors.push_back({0, 0, 0, 255}); // Fill with black (fully opaque)
    }

    m_numColors = static_cast<int>(m_colors.size());
    return 1;
}

std::string PalF::location()
{
    return m_location;
}

uint32_t PalF::nameSize()
{
    return m_nameSize;
}

std::vector<char> PalF::name()
{
    return m_name;
}

int PalF::colorModel()
{
    return m_colorModel;
}

int PalF::numColors()
{
    return m_numColors;
}

void PalF::location(std::string loc)
{
    m_location = loc;
}

void PalF::nameSize(uint32_t size)
{
    m_nameSize = size;
}

void PalF::name(std::vector<char> name)
{
    m_name = name;
}

void PalF::colorModel(int model)
{
    m_colorModel = model;
}

void PalF::numColors(int count)
{
    m_numColors = count;
}

ZtaColor PalF::getColor(int index)
{
    if (index < 0 || index >= m_numColors)
    {
        std::cerr << "ERROR: Color index out of bounds: " << index << std::endl;
        return {0, 0, 0, 255}; // Return black (fully opaque) as default
    }
    return m_colors[index];
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