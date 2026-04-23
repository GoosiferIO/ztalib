#include "ZtaF.h"

/* ZtaF.cpp -- parser for zt1 animation files

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

ZtaF::ZtaF()
{
    // init ZtaData
    _data = std::make_unique<ZtaData>();

    // init other members
    _file = std::ifstream();
    _file.exceptions(static_cast<std::ios_base::iostate>(
        std::ifstream::failbit | std::ifstream::badbit));
    colorModel = 0;

    _frameBuffer = std::vector<ZtaFrameBuffer::BufferObject>();
}

ZtaF::~ZtaF()
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

std::vector<ZtaFrameBuffer::BufferObject> ZtaF::getFrameBuffer()
{
    return _frameBuffer;
}

int ZtaF::getFrameCount()
{
    return _data->info.frameCount;
}

// ZtaColor model 0 = RGBA
// ZtaColor model 1 = BGRA
int ZtaF::load(std::string fileName, int colorModel, std::string ioPal)
{
    this->colorModel = colorModel;

    _file.open(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    if (!_file.is_open())
    {
        return -1;
    }

    // ------------------------------- read header
    // Note: if bg frame exists, not counted in frameCount
    // check if fatz
    if (ZtaUtils::hasMagic(_file))
    {
        // skip 8 bytes
        _file.seekg(8, std::ios::cur);
        // read 9th byte
        _file.read((char *)&_data->hasBackground, 1);
    } // else, not fatz (ztaf)

    _file.read((char *)&_data->info.speed, 4);                         // animation speed in ms
    _file.read((char *)&_data->palette->nameSize, 4);                  // size of palette name
    _data->palette->name.resize(_data->palette->nameSize);             // resize to size
    _file.read(_data->palette->name.data(), _data->palette->nameSize); // read palette name
    _file.read((char *)&_data->info.frameCount, 4);                    // number of frames
    _data->frames.resize(_data->info.frameCount);                      // resize frames to frame count

    if (ioPal.empty())
        _data->palette->location = std::string(_data->palette->name.data());
    else
        _data->palette->location = ioPal;

    if (_data->hasBackground)
    {
        _data->info.frameCount += 1;
        _data->frames.resize(_data->info.frameCount);
    }

    // ------------------------------- read palette
    _data->palette->read(_data->palette->location);

    // ------------------------------- read frames
    for (int i = 0; i < _data->info.frameCount; i++)
    {
        ZtaFrame frame = ZtaFrame();
        _file.read((char *)&frame.frameSize, 4);
        _file.read((char *)&frame.height, 2);
        _file.read((char *)&frame.width, 2);
        _file.read((char *)&frame.y, 2);
        _file.read((char *)&frame.x, 2);
        _file.read((char *)&frame.unk1, 1); // always 0?
        _file.read((char *)&frame.unk2, 1); // always 0?

        // read pixel sets
        for (int j = 0; j < frame.height; j++)
        {
            ZtaPixelSet pixelSet = ZtaPixelSet();
            _file.read((char *)&pixelSet.blockCount, 1); // how many pixel blocks
            pixelSet.blocks.resize(pixelSet.blockCount); // resize to block count
            for (int k = 0; k < pixelSet.blockCount; k++)
            { // read each block
                ZtaPixelBlock block = ZtaPixelBlock();
                _file.read((char *)&block.offset, 1);                      // offset
                _file.read((char *)&block.colorCount, 1);                  // color count
                block.colors.resize(block.colorCount);                     // resize to color count
                _file.read((char *)block.colors.data(), block.colorCount); // colors
                pixelSet.blocks[k] = std::move(block);                     // store block
            }

            // // TODO: test issues that might arise from this
            // // Possible issue: skips some pixels and leaves them blank
            // if (pixelSet->blockCount == 0) {
            //     pixelSet->blocks.push_back(std::make_unique<ZtaPixelBlock>(0, 0, std::vector<uint8_t>()));
            // }
            frame.pixelSets.push_back(pixelSet); // store pixel set
        }

        // store frame
        _data->frames[i] = std::move(frame);
    }

    _file.close();

    // write output buffer
    ZtaFrameBuffer apeFrameBuffer(*_data);
    _frameBuffer = apeFrameBuffer.getBuffer();

    if (_frameBuffer.empty())
    {
        return -2;
    }
    return 1;
}

int ZtaF::save(std::string fileName)
{
    std::ofstream output(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::out));
    if (!output.is_open())
    {
        return -1;
    }

    if (_data->hasBackground)
    {
        // WRITE: FATZ
        output.write(MAGIC, 4);
    }

    // -------------------------------------- write header
    output.write((char *)&_data->info.speed, 4);                         // speed in ms
    output.write((char *)&_data->palette->nameSize, 4);                  // size of palette name
    output.write(_data->palette->name.data(), _data->palette->nameSize); // palette name
    output.write((char *)&_data->info.frameCount, 4);                    // frame count

    // write frames
    // TODO: write a Buffer reader and convert to frames and pal
    for (const ZtaFrame &frame : _data->frames)
    {
        output.write((char *)&frame.frameSize, 4);
        output.write((char *)&frame.height, 2);
        output.write((char *)&frame.width, 2);
        output.write((char *)&frame.x, 2);
        output.write((char *)&frame.y, 2);
        output.write((char *)&frame.unk1, 1);
        output.write((char *)&frame.unk2, 1);

        // write pixel sets
        for (const ZtaPixelSet &pixelSet : frame.pixelSets)
        {
            output.write((char *)&pixelSet.blockCount, 1);
            for (const ZtaPixelBlock &block : pixelSet.blocks)
            {
                output.write((char *)&block.offset, 1);
                output.write((char *)&block.colorCount, 1);
                output.write((char *)block.colors.data(), block.colorCount);
            }
        }
    }

    output.close();

    // write palette
    // ZtaF::writePal(_data->palette->location);

    return 1;
}

// Does a simple validation to see if _file is valid ZTA graphic
// 1. Checks if _file is open = Not valid
// 2. Checks if first 4 bytes are FATZ = Valid
// 3. Checks if palette name is empty = Not valid
// 4. Checks if palette name has '.pal' extension = Valid
int ZtaF::validateGraphicFile(std::string fileName)
{
    std::ifstream graphic(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    int isValid = 0;
    PalF pal = PalF();
    ZtaInfo info = ZtaInfo();

    // if _file is not open, return false
    if (!graphic.is_open())
    {
        // immediately return false if _file is not open
        return 0;
    }

    // if has magic bytes FATZ
    if (ZtaUtils::hasMagic(graphic))
    {
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

    graphic.read((char *)&info.speed, 4);        // speed in ms
    graphic.read((char *)&pal.nameSize, 4);      // size of palette name
    pal.name.resize(pal.nameSize);               // resize to size
    graphic.read(pal.name.data(), pal.nameSize); // read palette name
    graphic.read((char *)&info.frameCount, 4);   // frame count

    // if pal name is empty, return false
    if (pal.name.empty() || pal.nameSize == 0 || pal.nameSize < 0)
    {
        // if no palette exists then immediately return false
        return 0;
    }

    std::string palette(pal.name.data());

    // if pal name has '.pal' extension, return true
    if (palette.find(".pal") != std::string::npos)
    {
        isValid = 1;
    }

    graphic.close();
    return isValid;
}

ZtaInfo ZtaF::getHeader(std::string fileName)
{
    return _data ? _data->info : ZtaInfo();
}

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