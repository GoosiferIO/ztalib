#ifndef APEPIXELSET_H
#define APEPIXELSET_H

#include "ZtaPixelBlock.h"

struct ZtaPixelSet 
{
    uint8_t blockCount; // How many pixel blocks
    std::vector<ZtaPixelBlock> blocks; // The pixel blocks
};

#endif // APEPIXELSET_H