#ifndef ZTAF_H
#define ZTAF_H

/* ZtaF.h -- parser for zt1 animation files

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

#include <fstream>
#include <vector>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <memory>

#include "../../libs/stb_image_write.h"
#include "../data/ZtaData.h"
#include "../data/ZtaFrameBuffer.h"
#include "../other/ZtaUtils.h"

#define ZTA_CORE_VERSION "0.6.4"

// if FATZ is first 4 bytes, additional 5 bytes ahead
// The ninth byte is a boolean value that specifies if there is an
// a background frame

// -------------------------------- Standard Pixel Output

class ZtaF
{
public:
    ZtaF();
    virtual ~ZtaF();

    int load(std::string fileName, int colorProfile = 0, std::string ioPal = "");
    int save(std::string fileName);
    int exportToPng(std::string fileName,
                    const ZtaFrameBuffer::BufferObject& output);
    int getFrameCount();
    std::vector<ZtaFrameBuffer::BufferObject> getFrameBuffer();
    std::string getPalLocation();
    static int validateGraphicFile(std::string fileName);
    int hasBackgroundFrame();
    // return header info
    ZtaInfo getHeader(std::string fileName);

private:
    // binary input
    std::ifstream _file;
    // output buffers
    std::vector<ZtaFrameBuffer::BufferObject> _frameBuffer;
    // data
    std::unique_ptr<ZtaData> _data;
    // other
    int colorModel;
};

#endif // ZTAF_H

/*
    MIT License

    Copyright (c) 2025 Goosifer.IO

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