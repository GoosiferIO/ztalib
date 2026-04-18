#pragma once
#include <vector>

struct ApeHeader
{
    uint32_t speed; // animation speed in ms
    uint32_t palNameSize;
    std::vector<char> palName;
    uint32_t frameCount;
};
