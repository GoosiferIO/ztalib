#ifndef APEFRAME_H
#define APEFRAME_H

#include <vector>
#include "ZtaPixelSet.h"

struct ZtaFrame
{
    uint32_t frameSize; // in bytes
    uint16_t height;
    uint16_t width;
    int16_t y; // y offset
    int16_t x; // x offset
    uint8_t unk1; // unknown bytes
    uint8_t unk2; // unknown bytes
    std::vector<ZtaPixelSet> pixelSets; // The pixel sets  
};

#endif // APE_FRAME_H