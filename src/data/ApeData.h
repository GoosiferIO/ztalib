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
        std::unique_ptr<ApeInfo> info;
        std::vector<std::unique_ptr<ApeFrame>> frames;
        std::unique_ptr<PalF> palette;
        bool hasBackground = false;

        ApeData();
};

ApeData::ApeData()
{
    info = std::unique_ptr<ApeInfo>(new ApeInfo());
    info->speed = 0;
    info->frameCount = 0;
    frames = std::vector<std::unique_ptr<ApeFrame>>();
    palette = std::unique_ptr<PalF>(new PalF());
}

#endif // APEDATA_H