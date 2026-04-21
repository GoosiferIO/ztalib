#ifndef APEDATA_H
#define APEDATA_H

#include <vector>
#include <memory>

// structs
#include "ApeFrame.h"
#include "ApeInfo.h"

// parsers
#include "../parsers/PalF.h"

class ApeData {
    public:
        ApeInfo info;
        std::vector<ApeFrame> frames;
        std::unique_ptr<PalF> palette;
        bool hasBackground = false;

        ApeData();
};

ApeData::ApeData()
{
    info.speed = 0;
    info.frameCount = 0;
    frames = std::vector<ApeFrame>();
    palette = std::make_unique<PalF>();
}

#endif // APEDATA_H