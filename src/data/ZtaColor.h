#ifndef APECOLOR_H
#define APECOLOR_H

#include <vector>
#include "ZtaPixelBlock.h"
#include <cstdint>

/*
Struct that contains color information for a single pixel in the PAL file.
*/

struct ZtaColor 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

#endif // APECOLOR_H