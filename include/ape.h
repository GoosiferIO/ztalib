#ifndef APE_H
#define APE_H

// Standard Libraries
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdint>
#include <cstring>
#include "stb_image_write.h"
#include <omp.h>

// Project
#include "outputbuffer.h"
#include "apeheader.h"
#include "frame.h"
#include "pixelblock.h"
#include "pixelset.h"
#include "color.h"

#define MAGIC "FATZ"
#define APE_CORE_VERSION "0.6.4"

class Ape
{
    public: 
        Ape();
        virtual ~Ape();

        int load(std::string fileName, int colorProfile = 0, std::string ioPal = "");
        int save(std::string fileName);
        int exportToPNG(std::string fileName, OutputBuffer output);
        int getFrameCount();
        OutputBuffer** apeBuffer();
        std::string getPalLocation();
        std::vector<Frame>& getFrames();
        std::vector<Color>& getColors() { return colors; }
        static int validateGraphicFile(std::string fileName);
        static int validatePaletteFile(std::string fileName);
        static int hasBackgroundFrame(std::string fileName);
        std::vector<size_t> computeFrameOffsets(std::vector<uint8_t> &data);
        size_t getFrameOffset(int index, std::vector<size_t> &offsets);
        // return header info
        static Header getHeader(std::string fileName);

    private:
        int readPal(std::string fileName);
        void writePal(std::string fileName);
        static int hasMagic(std::ifstream &input);
        int writeBuffer();

        std::ifstream input;
        std::ifstream pal;
        OutputBuffer** frameBuffers;
        Header header;
        std::vector<Frame> frames;
        std::vector<std::vector<PixelBlock>> pixelBlocks;
        std::vector<Color> colors;
        bool hasBackground;
        int colorModel;
        std::string palLocation;
};

#endif // APE_H