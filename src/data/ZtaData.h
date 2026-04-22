#ifndef ZTADATA_H
#define ZTADATA_H

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

#endif // ZTADATA_H