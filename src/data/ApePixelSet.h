#ifndef APEPIXELSET_H
#define APEPIXELSET_H

#include "ApePixelBlock.h"

struct ApePixelSet 
{
    uint8_t blockCount; // How many pixel blocks
    std::vector<ApePixelBlock> blocks; // The pixel blocks
};

#endif // APEPIXELSET_H