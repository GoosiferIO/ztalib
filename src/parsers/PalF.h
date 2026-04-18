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
        std::ifstream file;
        std::vector<ApeColor> colors;
        uint32_t nameSize;
        std::vector<char> name;
        std::string location;
};

PalF::PalF()
{
    file = std::ifstream();
    file.exceptions(static_cast<std::ios_base::iostate>(
        std::ifstream::failbit | std::ifstream::badbit));
    colors = std::vector<ApeColor>();
    nameSize = 0;
    name = std::vector<char>();
    location = "";
}