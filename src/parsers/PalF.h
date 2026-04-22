#ifndef PALF_H
#define PALF_H

#include <fstream>
#include <vector>
#include <iostream>
#include "../data/ZtaColor.h"

class PalF
{
public:
    PalF();
    int load(std::string fileName);
    virtual ~PalF();
    std::string location;
    uint32_t nameSize;
    std::vector<char> name;
    int colorModel;
    int numColors;
    ZtaColor getColor(int index);
    int read(std::string fileName);

private:
    std::ifstream file;
    std::vector<ZtaColor> colors;
};

#endif // PALF_H