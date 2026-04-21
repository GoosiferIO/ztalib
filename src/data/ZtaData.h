#ifndef APEDATA_H
#define APEDATA_H

#include <vector>
#include <memory>

// structs
#include "ZtaFrame.h"
#include "ZtaInfo.h"

// parsers
#include "../parsers/PalF.h"

class ZtaData {
    public:
        ZtaInfo info;
        std::vector<ZtaFrame> frames;
        std::unique_ptr<PalF> palette;
        bool hasBackground = false;

        ZtaData();
};

#endif // APEDATA_H