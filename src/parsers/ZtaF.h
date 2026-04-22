#ifndef ZTAF_H
#define ZTAF_H

// ape-graphics-parser - ZT1 Graphics Parser
// by Eric Galvan (Goosifer.IO)
// https://github.com/openztcc/ZTA.Core
// Licensed under MIT (see LICENSE)
//
// This library parses ZT1 game sprites and animations. Please feel
// free to use in your own projects and tools.
//
// Credits to Jeff Bostoen for his fantastic documentation on the ZT1 file formats:
// https://github.com/jbostoen/ZTStudio/wiki/ZT1-Graphics-Explained
//
// Credits to the original author of the STB image library (for PNG writing):
// - Sean Barrett (released under public domain/MIT)
//
// Version 0.6.3

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