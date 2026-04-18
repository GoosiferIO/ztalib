#pragma once
#include <vector>
#include "ApeFrame.h"
#include "ApeInfo.h"
#include "../parsers/PalF.h"
#include "ApeFrameBuffer.h"
#include "../parsers/PalF.h"

class ApeData {
    public:
        ApeInfo info;
        std::vector<ApeFrame> frames;
        PalF palette;
        bool hasBackground = false;

        ApeData();
};

ApeData::ApeData()
{
    info.speed = 0;
    info.frameCount = 0;
    frames = std::vector<ApeFrame>();
    palette = PalF();
}