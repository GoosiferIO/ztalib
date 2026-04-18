#include "ApeF.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

ApeF::ApeF()
{
    // init ApeData
    apef = std::make_unique<ApeData>(new ApeData());

    // init other members
    file = std::ifstream();
    file.exceptions(static_cast<std::ios_base::iostate>(
        std::ifstream::failbit | std::ifstream::badbit));
    colorModel = 0;

    frameBuffers = new ApeFrameBuffer*[1];
}

ApeF::~ApeF()
{ 
    if (file.is_open()) 
    {
        file.close();
    }

    // free frame buffers
    int numBuffers = getFrameCount();
    for (int i = 0; i < numBuffers; i++) 
    {
        delete[] frameBuffers[i]->pixels;
        delete frameBuffers[i];
    }

    // free frames
    apef->frames.clear();

    // free pixel blocks
    for (std::vector<ApePixelBlock> &blocks : apef->pixelBlocks) 
    {
        blocks.clear();
    }

    // free header
    apef->info.palName.clear();    
}

ApeFrameBuffer** ApeF::getFrameBuffers()
{
    return frameBuffers;
}

int ApeF::getFrameCount() 
{
    return apef->info.frameCount;
}

std::string ApeF::getPalLocation() 
{
    return apef->palette->location;
}


int ApeF::hasMagic(std::ifstream &file)
{
    char magic[5] = {0};
    file.read(magic, 4);

    // std::cout << "\tMagic Bytes: " << magic << std::endl;
    
    // read at least 4 bytes
    // if less than 4 bytes, not FATZ
    if (file.gcount() < 4) 
    {
        file.clear();  
        file.seekg(0, std::ios::beg);
        return 0;
    }

    // test for FATZ
    if (strcmp(magic, MAGIC) != 0) {
        file.clear();
        file.seekg(0, std::ios::beg);
        return 0;
    }

    // FATZ found
    file.clear();
    file.seekg(0, std::ios::beg);
    return 1;
}

int ApeF::writeBuffer() 
{
    if (frames.empty()) {
        return 0;
    }

    int numBuffers = getFrameCount();
    frameBuffers = new ApeFrameBuffer*[numBuffers];

    for (ApeFrame &frame : frames) 
    {
        int index = &frame - &frames[0];
        ApeFrameBuffer output = ApeFrameBuffer();

        // Set dimensions and format
        output.width = static_cast<int>(frame.width);
        output.height = static_cast<int>(frame.height);
        output.offsetX = static_cast<int>(frame.x);
        output.offsetY = static_cast<int>(frame.y);
        output.channels = 4;  // RGBA/BGRA
        
        // Calculate buffer size and initialize with transparent pixels
        size_t bufferSize = output.width * output.height * output.channels;
        output.pixels = new uint8_t[bufferSize];
        for (size_t i = 0; i < bufferSize; i += 4) {
            output.pixels[i] = 0;     // R
            output.pixels[i + 1] = 0; // G
            output.pixels[i + 2] = 0; // B
            output.pixels[i + 3] = 0; // A (Fully transparent)
        }

        // Process each row
        for (int row = 0; row < frame.height; row++) 
        {
            if (row >= frame.pixelSets.size()) {
                std::cerr << "ERROR: Row " << row << " exceeds ApePixelSet count!" << std::endl;
                continue;
            }

            ApePixelSet &ApePixelSet = frame.pixelSets[row];
            int xPos = 0;  // Reset horizontal position for each new row

            // Process each block in the row
            for (ApePixelBlock &ApePixelBlock : ApePixelSet.blocks) 
            {
                // Apply offset from current position
                xPos += ApePixelBlock.offset;

                // Skip if this is an end-of-line block or empty block
                if (ApePixelBlock.colorCount == 0) {
                    continue;
                }

                // Process each color in the block
                for (uint8_t colorIndex : ApePixelBlock.colors) 
                {
                    // Bounds checking
                    if (xPos >= output.width) {
                        break;  // Stop if we've exceeded the width
                    }

                    // Validate color index
                    if (colorIndex >= colors.size()) {
                        std::cerr << "ERROR: Out-of-bounds color index! (" 
                                 << (int)colorIndex << ")" << std::endl;
                        continue;
                    }

                    // Calculate pixel position in buffer
                    size_t pixelIndex = (row * output.width + xPos) * output.channels;
                    
                    // Ensure we don't write outside the buffer
                    if (pixelIndex + 3 >= bufferSize) {
                        std::cerr << "ERROR: Buffer overflow prevented at position " 
                                 << pixelIndex << std::endl;
                        break;
                    }

                    // Get color from palette
                    ApeColor &color = colors[colorIndex];

                    // Write pixel data according to color model
                    if (colorModel == 1) {  // BGRA mode
                        output.pixels[pixelIndex] = color.b;
                        output.pixels[pixelIndex + 1] = color.g;
                        output.pixels[pixelIndex + 2] = color.r;
                        output.pixels[pixelIndex + 3] = color.a;
                    } else {  // RGBA mode
                        output.pixels[pixelIndex] = color.r;
                        output.pixels[pixelIndex + 1] = color.g;
                        output.pixels[pixelIndex + 2] = color.b;
                        output.pixels[pixelIndex + 3] = color.a;
                    }
                    
                    xPos++;  // Move to next horizontal position
                }
            }
        }
        
        // Store the completed buffer
        frameBuffers[index] = new ApeFrameBuffer(output);
    }

    return 1;
}// ApeColor model 0 = RGBA
// ApeColor model 1 = BGRA
int ApeF::load(std::string fileName, int colorModel, std::string ioPal)
{
    this->colorModel = colorModel;

    file.open(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    if (!file.is_open()) {
        return -1;
    }

    // ------------------------------- read header
    // Note: if bg frame exists, not counted in frameCount
    std::cout << "Header" << std::endl;

    // check if fatz
    if (hasMagic(file)) {
        // skip 8 bytes
        file.seekg(8, std::ios::cur);
        // read 9th byte
        file.read((char*)&hasBackground, 1);
        std::cout << "\tType: is fatz" << std::endl;
        std::cout << "\thasBackground: " << hasBackground << std::endl;
    } else {
        std::cout << "\tType: not fatz" << std::endl;
    }

    file.read((char*)&header.speed, 4); // animation speed in ms
    file.read((char*)&header.palNameSize, 4); // size of palette name
    header.palName.resize(header.palNameSize); // resize to size
    file.read(header.palName.data(), header.palNameSize); // read palette name
    file.read((char*)&header.frameCount, 4); // number of frames
    frames.resize(header.frameCount); // resize frames to frame count

    if (ioPal.empty()) 
        apef->palette->location = std::string(header.palName.data());
    else
        apef->palette->location = ioPal;

    if (hasBackground) {
        header.frameCount += 1;
        frames.resize(header.frameCount);
    }

    // print header
    std::cout << "\tspeed: " << header.speed << " ms" << std::endl;
    std::cout << "\tpalNameSize: " << header.palNameSize << " bytes" << std::endl;
    std::cout << "\tpalName: " << header.palName.data() << std::endl;
    std::cout << "\tframeCount: " << header.frameCount << std::endl;
    std::cout << "\tframes: " << frames.size() << std::endl;

    // ------------------------------- read palette
    ApeF::readPal(apef->palette->location);

    // ------------------------------- read frames
    for (int i = 0; i < header.frameCount; i++) {
        ApeFrame frame;
        file.read((char*)&frame.frameSize, 4);
        file.read((char*)&frame.height, 2);
        file.read((char*)&frame.width, 2);
        file.read((char*)&frame.y, 2);
        file.read((char*)&frame.x, 2);
        file.read((char*)&frame.unk1, 1); // always 0?
        file.read((char*)&frame.unk2, 1); // always 0?

        // read pixel sets
        for (int j = 0; j < frame.height; j++) {
            ApePixelSet ApePixelSet;
            file.read((char*)&ApePixelSet.blockCount, 1); // how many pixel blocks
            ApePixelSet.blocks.resize(ApePixelSet.blockCount); // resize to block count
            for (int k = 0; k < ApePixelSet.blockCount; k++) { // read each block
                ApePixelBlock block;
                file.read((char*)&block.offset, 1); // offset
                file.read((char*)&block.colorCount, 1); // color count
                block.colors.resize(block.colorCount); // resize to color count
                file.read((char*)block.colors.data(), block.colorCount); // colors
                ApePixelSet.blocks[k] = block; // store block
            }

            // // TODO: test issues that might arise from this
            // // Possible issue: skips some pixels and leaves them blank
            // if (ApePixelSet.blockCount == 0) {
            //     ApePixelSet.blocks.push_back(ApePixelBlock{0, 0, std::vector<uint8_t>()});
            // }
            frame.pixelSets.push_back(ApePixelSet); // store pixel set
        }

        // store frame
        frames[i] = frame;

        // print frame
        std::cout << "ApeFrame " << i << std::endl;
        std::cout << "\tframeSize: " << frame.frameSize << " bytes" << std::endl;
        std::cout << "\theight: " << (int)frame.height << " px" << std::endl;
        std::cout << "\twidth: " << (int)frame.width << " px" << std::endl;
        std::cout << "\ty: " << (int)frame.y << std::endl;
        std::cout << "\tx: " << (int)frame.x << std::endl;
        std::cout << "\tunk1: " << (int)frame.unk1 << std::endl;
        std::cout << "\tunk2: " << (int)frame.unk2 << std::endl;
        std::cout << "\tApePixelSets: " << frame.pixelSets.size() << std::endl;
        for (int j = 0; j < frame.pixelSets.size(); j++) {
            std::cout << "\t\tApePixelSet " << j << std::endl;
            std::cout << "\t\t\tblockCount: " << (int)frame.pixelSets[j].blockCount << std::endl;
            for (int k = 0; k < frame.pixelSets[j].blocks.size(); k++) {
                std::cout << "\t\t\tblock " << k << std::endl;
                std::cout << "\t\t\t\toffset: " << (int)frame.pixelSets[j].blocks[k].offset << std::endl;
                std::cout << "\t\t\t\tcolorCount: " << (int)frame.pixelSets[j].blocks[k].colorCount << std::endl;
                std::cout << "\t\t\t\tcolors: ";
                for (int l = 0; l < frame.pixelSets[j].blocks[k].colorCount; l++) {
                    std::cout << (int)frame.pixelSets[j].blocks[k].colors[l] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    file.close();

    // write output buffer
    if (ApeF::writeBuffer() > 0) {
        std::cout << "Wrote output buffer" << std::endl;
    } else if (ApeF::writeBuffer() == 0) {
        std::cout << "No frames to write" << std::endl;
    } else {
        std::cout << "Failed to write output buffer" << std::endl;
    }

    return 1;
}

int ApeF::save(std::string fileName)
{
    std::ofstream output(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::out));
    if (!output.is_open()) {
        return -1;
    }

    if (hasBackground) 
    {
        // WRITE: FATZ
        output.write(MAGIC, 4);
    }

    // -------------------------------------- write header
    output.write((char*)&header.speed, 4); // speed in ms
    output.write((char*)&header.palNameSize, 4); // size of palette name
    output.write(header.palName.data(), header.palNameSize); // palette name
    output.write((char*)&header.frameCount, 4); // frame count

    // write frames
    // TODO: write a Buffer reader and convert to frames and pal
    for (ApeFrame &frame : frames) {
        output.write((char*)&frame.frameSize, 4);
        output.write((char*)&frame.height, 2);
        output.write((char*)&frame.width, 2);
        output.write((char*)&frame.x, 2);
        output.write((char*)&frame.y, 2);
        output.write((char*)&frame.unk1, 1);
        output.write((char*)&frame.unk2, 1);

        // write pixel sets
        for (ApePixelSet &ApePixelSet : frame.pixelSets) {
            output.write((char*)&ApePixelSet.blockCount, 1);
            for (ApePixelBlock &block : ApePixelSet.blocks) {
                output.write((char*)&block.offset, 1);
                output.write((char*)&block.colorCount, 1);
                output.write((char*)block.colors.data(), block.colorCount);
            }
        }
    }

    output.close();

    // write palette
    // ApeF::writePal(apef->palette->location);

    return 1;
}

// Does a simple validation to see if file is valid APE graphic
// 1. Checks if file is open = Not valid
// 2. Checks if first 4 bytes are FATZ = Valid
// 3. Checks if palette name is empty = Not valid
// 4. Checks if palette name has '.pal' extension = Valid
int ApeF::validateGraphicFile(std::string fileName) 
{
    std::ifstream graphic(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    int isValid = 0;
    ApeInfo hdr;
    
    // if file is not open, return false
    if (!graphic.is_open()) {
        // immediately return false if file is not open
        return 0;
    }

    // if has magic bytes FATZ
    if (hasMagic(graphic)) {
        isValid = 1;

        // skip 9 bytes
        graphic.seekg(9, std::ios::cur);
    }
    // if no magic bytes, it means first 9 bytes don't exist
    // following checks are not necessary:
    // - MAGIC BYTES - 4 bytes
    // - UNKNOWN NULL BYTES - 4 bytes
    // - HAS BACKGROUND - 1 byte
    // does not mean file is invalid, just not a FATZ file or ZTAF (reverse FATZ)

    graphic.read((char*)&hdr.speed, 4); // speed in ms
    graphic.read((char*)&hdr.palNameSize, 4); // size of palette name
    hdr.palName.resize(hdr.palNameSize); // resize to size
    graphic.read(hdr.palName.data(), hdr.palNameSize); // read palette name
    graphic.read((char*)&hdr.frameCount, 4); // frame count

    // if pal name is empty, return false
    if (hdr.palName.empty() || hdr.palNameSize == 0 || hdr.palNameSize < 0) {
        // if no palette exists then immediately return false
        return 0;
    }

    std::string palette(hdr.palName.data());

    // if pal name has '.pal' extension, return true
    if (palette.find(".pal") != std::string::npos) {
        isValid = 1;
    }

    graphic.close();
    return isValid;
}

int ApeF::hasBackgroundFrame() 
{
    return hasBackground;
}

int ApeF::exportToPNG(std::string fileName, ApeFrameBuffer output)
{
    if (!output.pixels) {
        std::cerr << "No pixels to write" << std::endl;
        return -1;
    }

    if (!stbi_write_png(fileName.c_str(), output.width, output.height, output.channels, output.pixels, 0)) {
        std::cerr << "Failed to write image" << std::endl;
        return -2;
    } else {
        std::cout << "Wrote image" << std::endl;
    }

    return 1;
}

ApeInfo ApeF::getHeader(std::string fileName) 
{
    std::ifstream graphic(fileName, std::ios::binary);
    ApeInfo hdr;
    if (!graphic.is_open()) {
        return hdr;
    }

    // if has magic bytes FATZ
    if (hasMagic(graphic)) {
        // skip 9 bytes
        graphic.seekg(9, std::ios::cur);
    }

    graphic.read((char*)&hdr.speed, 4); // speed in ms
    graphic.read((char*)&hdr.palNameSize, 4); // size of palette name
    hdr.palName.resize(hdr.palNameSize); // resize to size
    graphic.read(hdr.palName.data(), hdr.palNameSize); // read palette name
    graphic.read((char*)&hdr.frameCount, 4); // frame count

    graphic.close();
    return hdr;
}