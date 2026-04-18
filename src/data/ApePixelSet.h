#pragma once
#include "ApePixelBlock.h"

struct ApePixelSet 
{
    uint8_t blockCount; // How many pixel blocks
    std::vector<ApePixelBlock> blocks; // The pixel blocks
};
