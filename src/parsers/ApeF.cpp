#include "ApeF.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

ApeF::ApeF()
{
    // init ApeData
    _data = std::make_unique<ApeData>(new ApeData());

    // init other members
    _file = std::ifstream();
    _file.exceptions(static_cast<std::ios_base::iostate>(
        std::ifstream::failbit | std::ifstream::badbit));
    colorModel = 0;

    _frameBuffer = std::vector<std::unique_ptr<ApeFrameBuffer::BufferObject>>();
}

ApeF::~ApeF()
{ 
    if (_file.is_open()) 
    {
        _file.close();
    }

    // free frames
    _data->frames.clear();

    // free header
    _data->palette->name.clear();    
}

std::vector<std::unique_ptr<ApeFrameBuffer::BufferObject>> ApeF::getFrameBuffer()
{
    return _frameBuffer;
}

int ApeF::getFrameCount() 
{
    return _data->info->frameCount;
}

std::string ApeF::getPalLocation() 
{
    return _data->palette->location;
}

// ApeColor model 0 = RGBA
// ApeColor model 1 = BGRA
int ApeF::load(std::string fileName, int colorModel, std::string ioPal)
{
    this->colorModel = colorModel;

    _file.open(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    if (!_file.is_open()) {
        return -1;
    }

    // ------------------------------- read header
    // Note: if bg frame exists, not counted in frameCount
    std::cout << "Header" << std::endl;

    // check if fatz
    if (ApeUtils::hasMagic(_file)) {
        // skip 8 bytes
        _file.seekg(8, std::ios::cur);
        // read 9th byte
        _file.read((char*)&_data->hasBackground, 1);
        std::cout << "\tType: is fatz" << std::endl;
        std::cout << "\thasBackground: " << _data->hasBackground << std::endl;
    } else {
        std::cout << "\tType: not fatz" << std::endl;
    }

    _file.read((char*)&_data->info->speed, 4); // animation speed in ms
    _file.read((char*)&_data->palette->nameSize, 4); // size of palette name
    _data->palette->name.resize(_data->palette->nameSize); // resize to size
    _file.read(_data->palette->name.data(), _data->palette->nameSize); // read palette name
    _file.read((char*)&_data->info->frameCount, 4); // number of frames
    _data->frames.resize(_data->info->frameCount); // resize frames to frame count

    if (ioPal.empty()) 
        _data->palette->location = std::string(_data->palette->name.data());
    else
        _data->palette->location = ioPal;

    if (_data->hasBackground) {
        _data->info->frameCount += 1;
        _data->frames.resize(_data->info->frameCount);
    }

    // print header
    std::cout << "\tspeed: " << _data->info->speed << " ms" << std::endl;
    std::cout << "\tpalNameSize: " << _data->palette->nameSize << " bytes" << std::endl;
    std::cout << "\tpalName: " << _data->palette->name.data() << std::endl;
    std::cout << "\tframeCount: " << _data->info->frameCount << std::endl;
    std::cout << "\tframes: " << _data->frames.size() << std::endl;

    // ------------------------------- read palette
    _data->palette->read(_data->palette->location);

    // ------------------------------- read frames
    for (int i = 0; i < _data->info->frameCount; i++) {
        std::unique_ptr<ApeFrame> frame = std::make_unique<ApeFrame>();
        _file.read((char*)&frame->frameSize, 4);
        _file.read((char*)&frame->height, 2);
        _file.read((char*)&frame->width, 2);
        _file.read((char*)&frame->y, 2);
        _file.read((char*)&frame->x, 2);
        _file.read((char*)&frame->unk1, 1); // always 0?
        _file.read((char*)&frame->unk2, 1); // always 0?

        // read pixel sets
        for (int j = 0; j < frame->height; j++) {
            std::unique_ptr<ApePixelSet> pixelSet = std::make_unique<ApePixelSet>();
            _file.read((char*)&pixelSet->blockCount, 1); // how many pixel blocks
            pixelSet->blocks.resize(pixelSet->blockCount); // resize to block count
            for (int k = 0; k < pixelSet->blockCount; k++) { // read each block
                std::unique_ptr<ApePixelBlock> block = std::make_unique<ApePixelBlock>();
                _file.read((char*)&block->offset, 1); // offset
                _file.read((char*)&block->colorCount, 1); // color count
                block->colors.resize(block->colorCount); // resize to color count
                _file.read((char*)block->colors.data(), block->colorCount); // colors
                pixelSet->blocks[k] = std::move(block); // store block
            }

            // // TODO: test issues that might arise from this
            // // Possible issue: skips some pixels and leaves them blank
            // if (pixelSet->blockCount == 0) {
            //     pixelSet->blocks.push_back(std::make_unique<ApePixelBlock>(0, 0, std::vector<uint8_t>()));
            // }
            frame->pixelSets.push_back(pixelSet); // store pixel set
        }

        // store frame
        _data->frames[i] = std::move(frame);

        // print frame
        std::cout << "ApeFrame " << i << std::endl;
        std::cout << "\tframeSize: " << frame->frameSize << " bytes" << std::endl;
        std::cout << "\theight: " << (int)frame->height << " px" << std::endl;
        std::cout << "\twidth: " << (int)frame->width << " px" << std::endl;
        std::cout << "\ty: " << (int)frame->y << std::endl;
        std::cout << "\tx: " << (int)frame->x << std::endl;
        std::cout << "\tunk1: " << (int)frame->unk1 << std::endl;
        std::cout << "\tunk2: " << (int)frame->unk2 << std::endl;
        std::cout << "\tApePixelSets: " << frame->pixelSets.size() << std::endl;
        for (int j = 0; j < frame->pixelSets.size(); j++) {
            std::cout << "\t\tApePixelSet " << j << std::endl;
            std::cout << "\t\t\tblockCount: " << (int)frame->pixelSets[j]->blockCount << std::endl;
            for (int k = 0; k < frame->pixelSets[j]->blocks.size(); k++) {
                std::cout << "\t\t\tblock " << k << std::endl;
                std::cout << "\t\t\t\toffset: " << (int)frame->pixelSets[j]->blocks[k]->offset << std::endl;
                std::cout << "\t\t\t\tcolorCount: " << (int)frame->pixelSets[j]->blocks[k]->colorCount << std::endl;
                std::cout << "\t\t\t\tcolors: ";
                for (int l = 0; l < frame->pixelSets[j]->blocks[k]->colorCount; l++) {
                    std::cout << (int)frame->pixelSets[j]->blocks[k]->colors[l] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    _file.close();

    // write output buffer
    std::unique_ptr<ApeFrameBuffer> apeFrameBuffer 
    = std::make_unique<ApeFrameBuffer>(*_data);
    _frameBuffer = apeFrameBuffer->getBuffer();
    if (_frameBuffer.empty()) 
    {
        return -2;
    }
    return 1;
}

int ApeF::save(std::string fileName)
{
    std::ofstream output(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::out));
    if (!output.is_open()) {
        return -1;
    }

    if (_data->hasBackground) 
    {
        // WRITE: FATZ
        output.write(MAGIC, 4);
    }

    // -------------------------------------- write header
    output.write((char*)&_data->info->speed, 4); // speed in ms
    output.write((char*)&_data->palette->nameSize, 4); // size of palette name
    output.write(_data->palette->name.data(), _data->palette->nameSize); // palette name
    output.write((char*)&_data->info->frameCount, 4); // frame count

    // write frames
    // TODO: write a Buffer reader and convert to frames and pal
    for (const std::unique_ptr<ApeFrame>& frame : _data->frames) {
        output.write((char*)&frame->frameSize, 4);
        output.write((char*)&frame->height, 2);
        output.write((char*)&frame->width, 2);
        output.write((char*)&frame->x, 2);
        output.write((char*)&frame->y, 2);
        output.write((char*)&frame->unk1, 1);
        output.write((char*)&frame->unk2, 1);

        // write pixel sets
        for (const std::unique_ptr<ApePixelSet>& pixelSet : frame->pixelSets) {
            output.write((char*)&pixelSet->blockCount, 1);
            for (const std::unique_ptr<ApePixelBlock>& block : pixelSet->blocks) {
                output.write((char*)&block->offset, 1);
                output.write((char*)&block->colorCount, 1);
                output.write((char*)block->colors.data(), block->colorCount);
            }
        }
    }

    output.close();

    // write palette
    // ApeF::writePal(_data->palette->location);

    return 1;
}

// Does a simple validation to see if _file is valid APE graphic
// 1. Checks if _file is open = Not valid
// 2. Checks if first 4 bytes are FATZ = Valid
// 3. Checks if palette name is empty = Not valid
// 4. Checks if palette name has '.pal' extension = Valid
int ApeF::validateGraphicFile(std::string fileName) 
{
    std::ifstream graphic(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    int isValid = 0;
    std::unique_ptr<PalF> pal = std::make_unique<PalF>(new PalF());
    std::unique_ptr<ApeInfo> info = std::make_unique<ApeInfo>(new ApeInfo());
    
    // if _file is not open, return false
    if (!graphic.is_open()) {
        // immediately return false if _file is not open
        return 0;
    }

    // if has magic bytes FATZ
    if (ApeUtils::hasMagic(graphic)) {
        isValid = 1;

        // skip 9 bytes
        graphic.seekg(9, std::ios::cur);
    }
    // if no magic bytes, it means first 9 bytes don't exist
    // following checks are not necessary:
    // - MAGIC BYTES - 4 bytes
    // - UNKNOWN NULL BYTES - 4 bytes
    // - HAS BACKGROUND - 1 byte
    // does not mean _file is invalid, just not a FATZ _file or ZTAF (reverse FATZ)

    graphic.read((char*)&info->speed, 4); // speed in ms
    graphic.read((char*)&pal->nameSize, 4); // size of palette name
    pal->name.resize(pal->nameSize); // resize to size
    graphic.read(pal->name.data(), pal->nameSize); // read palette name
    graphic.read((char*)&info->frameCount, 4); // frame count

    // if pal name is empty, return false
    if (pal->name.empty() || pal->nameSize == 0 || pal->nameSize < 0) {
        // if no palette exists then immediately return false
        return 0;
    }

    std::string palette(pal->name.data());

    // if pal name has '.pal' extension, return true
    if (palette.find(".pal") != std::string::npos) {
        isValid = 1;
    }

    graphic.close();
    return isValid;
}

int ApeF::hasBackgroundFrame() 
{
    return _data->hasBackground;
}

int ApeF::exportToPng(
    std::string fileName, 
    std::unique_ptr<ApeFrameBuffer::BufferObject> output)
{
    if (!output->pixels) {
        std::cerr << "No pixels to write" << std::endl;
        return -1;
    }

    if (!stbi_write_png(fileName.c_str(), output->width, output->height, output->channels, output->pixels, 0)) {
        std::cerr << "Failed to write image" << std::endl;
        return -2;
    } else {
        std::cout << "Wrote image" << std::endl;
    }

    return 1;
}

std::unique_ptr<ApeInfo> ApeF::getHeader(std::string fileName) 
{
    return _data->info ? std::make_unique<ApeInfo>(new ApeInfo(*_data->info)) : nullptr;
}