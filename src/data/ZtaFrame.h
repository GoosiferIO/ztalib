#ifndef ZTAFRAME_H
#define ZTAFRAME_H

/* ZtaFrame.h -- data for single frame

    ztalib - ZT1 graphics parser
    https://goosifer.io/
    Copyright 2025 Eric Galvan (GoosiferIO)

    OFFICIAL REPOSITORY

        https://github.com/GoosiferIO/ztalib

    LICENSE

        MIT License (see LICENSE at end of file or at project root)

    ACKNOWLEDGMENTS

        Thank you to Jeff Bostoen for his documentation on the ZT1 file formats:
        https://github.com/jbostoen/ZTStudio/wiki/ZT1-Graphics-Explained
*/

#include <vector>
#include "ZtaPixelSet.h"

struct ZtaFrame
{
    uint32_t frameSize; // in bytes
    uint16_t height;
    uint16_t width;
    int16_t y; // y offset
    int16_t x; // x offset
    uint8_t unk1; // unknown bytes
    uint8_t unk2; // unknown bytes
    std::vector<ZtaPixelSet> pixelSets; // The pixel sets  
};

#endif // ZTA_FRAME_H

/*
    MIT License

    Copyright (c) 2025 Eric Galvan (GoosiferIO)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/