#ifndef APEPIXELBLOCK_H
#define APEPIXELBLOCK_H

#include <vector>
#include <cstdint>

struct ZtaPixelBlock 
{
    uint8_t offset; // How many transparent pixels before drawing
    uint8_t colorCount; // How many colors in this block
    std::vector<uint8_t> colors; // Color indexes in pal
};

#endif // APEPIXELBLOCK_H
