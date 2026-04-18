#ifndef PIXELBLOCK_H
#define PIXELBLOCK_H

#include <cstdint>
#include <vector>

struct PixelBlock 
{
    uint8_t offset; // How many transparent pixels before drawing
    uint8_t colorCount; // How many colors in this block
    std::vector<uint8_t> colors; // Color indexes in pal
};

#endif // PIXELBLOCK_H