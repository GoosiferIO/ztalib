#include "ApeData.h"

ApeData::ApeData()
{
    info.speed = 0;
    info.frameCount = 0;
    frames = std::vector<ApeFrame>();
    palette = std::make_unique<PalF>();
}

