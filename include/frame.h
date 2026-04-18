#ifndef FRAME_H
#define FRAME_H

#include <cstdint>
#include <vector>
#include "pixelset.h"

struct Frame
{
    uint32_t frameSize; // in bytes
    uint16_t height;
    uint16_t width;
    int16_t y; // y offset
    int16_t x; // x offset
    uint8_t unk1; // unknown bytes
    uint8_t unk2; // unknown bytes
    std::vector<PixelSet> pixelSets; // The pixel sets  
};

#endif // FRAME_H