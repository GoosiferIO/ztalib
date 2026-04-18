#ifndef APEHEADER_H
#define APEHEADER_H

#include <cstdint>
#include <vector>

struct Header
{
    uint32_t speed; // animation speed in ms
    uint32_t palNameSize;
    std::vector<char> palName;
    uint32_t frameCount;
};

#endif // APEHEADER_H
