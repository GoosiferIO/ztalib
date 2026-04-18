#pragma once
#include <vector>
#include "ApeFrame.h"
#include "ApeInfo.h"
#include "../parsers/PalF.h"
#include "ApeFrameBuffer.h"
#include "../parsers/PalF.h"

struct ApeData {
    ApeInfo info;
    std::vector<ApeFrame> frames;
    PalF palette;
    bool hasBackground = false;

};