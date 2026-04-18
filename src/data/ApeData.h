#pragma once
#include <vector>
#include "ApeFrame.h"
#include "ApeHeader.h"
#include "../parsers/PalF.h"
#include "ApeFrameBuffer.h"

struct ApeData {
    ApeHeader header;
    std::vector<ApeFrame> frames;
    std::vector<ApePalette> palettes;
    std::vector<std::string> frameNames;
    std::vector<std::string> paletteNames;
};