#ifndef APEF_H
#define APEF_H

// ape-graphics-parser - ZT1 Graphics Parser
// by Eric Galvan (Goosifer.IO)
// https://github.com/openztcc/APE.Core
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

#include "./include/stb_image_write.h"
#include "../data/ApeData.h"

#define MAGIC "FATZ"
#define APE_CORE_VERSION "0.6.4"

// if FATZ is first 4 bytes, additional 5 bytes ahead
// The ninth byte is a boolean value that specifies if there is an 
// a background frame 


// -------------------------------- Standard Pixel Output

class ApeF
{
    public: 
        ApeF();
        virtual ~ApeF();

        int load(std::string fileName, int colorProfile = 0, std::string ioPal = "");
        int save(std::string fileName);
        int exportToPNG(std::string fileName, ApeFrameBuffer output);
        int getFrameCount();
        ApeFrameBuffer** getFrameBuffers();
        std::string getPalLocation();
        static int validateGraphicFile(std::string fileName);
        int hasBackgroundFrame();
        // return header info
        static ApeInfo getHeader(std::string fileName);

    private:
        static int hasMagic(std::ifstream &input);
        int writeBuffer();
        // binary input
        std::ifstream file;
        // output buffers
        ApeFrameBuffer** frameBuffers;
        // data
        std::unique_ptr<ApeData> apef;
        // other
        int colorModel;
    };

#endif // APEF_H