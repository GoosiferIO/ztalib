#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include "../data/ApeColor.h"

class PalF {
    public: 
        PalF();
        ~PalF();
        int load(std::string fileName);
        virtual ~PalF();
        static int validatePaletteFile(std::string fileName);

    private:
        int readPal(std::string fileName);
        std::ifstream palFile;
        std::vector<ApeColor> colors;
};