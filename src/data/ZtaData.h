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

#endif // APEDATA_H