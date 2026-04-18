#ifndef PIXELSET_H
#define PIXELSET_H

#include <cstdint>
#include <vector>
#include "pixelblock.h"

struct PixelSet 
{
    uint8_t blockCount; // How many pixel blocks
    std::vector<PixelBlock> blocks; // The pixel blocks
};

#endif // PIXELSET_H