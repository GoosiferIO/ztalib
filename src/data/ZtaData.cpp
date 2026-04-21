#include "ZtaData.h"

ZtaData::ZtaData()
{
    info.speed = 0;
    info.frameCount = 0;
    frames = std::vector<ZtaFrame>();
    palette = std::make_unique<PalF>();
}

