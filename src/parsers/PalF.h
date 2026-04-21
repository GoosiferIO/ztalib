#ifndef PALF_H
#define PALF_H

#include <fstream>
#include <vector>
#include <iostream>
#include "../data/ApeColor.h"

class PalF
{
public:
    PalF();
    int load(std::string fileName);
    virtual ~PalF();
    static int validatePaletteFile(std::string fileName);
    std::string location;
    uint32_t nameSize;
    std::vector<char> name;
    int colorModel;
    int numColors;
    ApeColor getColor(int index);
    int read(std::string fileName);

private:
    std::ifstream file;
    std::vector<ApeColor> colors;
};

#endif // PALF_H