#ifndef OUTPUTBUFFER_H
#define OUTPUTBUFFER_H

#include <cstdint>

struct OutputBuffer 
{
    uint8_t* pixels; // continuous array of pixels: i.e. {0,0,0,255,255,255,255,...}
    int width;
    int height;
    int offsetX;
    int offsetY;
    int channels;
};

#endif // OUTPUTBUFFER_H