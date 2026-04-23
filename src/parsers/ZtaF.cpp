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
    _data = std::make_shared<ZtaData>();
    colorModel = 0;
    _frameBuffer = std::vector<ZtaFrameBuffer::BufferObject>();
}

ZtaF::~ZtaF()
{
    // free frames
    _data->frames.clear();

    // free header
    _data->palette->name.clear();
}

std::vector<ZtaFrameBuffer::BufferObject> ZtaF::getFrameBuffer()
{
    return _frameBuffer;
}

// ZtaColor model 0 = RGBA
// ZtaColor model 1 = BGRA
std::shared_ptr<ZtaData> ZtaF::load(std::string fileName, int colorModel, std::string ioPal)
{
    this->colorModel = colorModel;
    std::ifstream file;
    file.open(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    if (!file.is_open())
    {
        return nullptr;
    }

    // ------------------------------- read header
    // Note: if bg frame exists, not counted in frameCount
    // check if fatz
    if (ZtaUtils::hasMagic(file))
    {
        // skip 8 bytes
        file.seekg(8, std::ios::cur);
        // read 9th byte
        file.read((char *)&_data->hasBackground, 1);
    } // else, not fatz (ztaf)

    file.read((char *)&_data->info.speed, 4);                         // animation speed in ms
    file.read((char *)&_data->palette->nameSize, 4);                  // size of palette name
    _data->palette->name.resize(_data->palette->nameSize);             // resize to size
    file.read(_data->palette->name.data(), _data->palette->nameSize); // read palette name
    file.read((char *)&_data->info.frameCount, 4);                    // number of frames
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
        file.read((char *)&frame.frameSize, 4);
        file.read((char *)&frame.height, 2);
        file.read((char *)&frame.width, 2);
        file.read((char *)&frame.y, 2);
        file.read((char *)&frame.x, 2);
        file.read((char *)&frame.unk1, 1); // always 0?
        file.read((char *)&frame.unk2, 1); // always 0?

        // read pixel sets
        for (int j = 0; j < frame.height; j++)
        {
            ZtaPixelSet pixelSet = ZtaPixelSet();
            file.read((char *)&pixelSet.blockCount, 1); // how many pixel blocks
            pixelSet.blocks.resize(pixelSet.blockCount); // resize to block count
            for (int k = 0; k < pixelSet.blockCount; k++)
            { // read each block
                ZtaPixelBlock block = ZtaPixelBlock();
                file.read((char *)&block.offset, 1);                      // offset
                file.read((char *)&block.colorCount, 1);                  // color count
                block.colors.resize(block.colorCount);                     // resize to color count
                file.read((char *)block.colors.data(), block.colorCount); // colors
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

    file.close();

    // write output buffer
    ZtaFrameBuffer apeFrameBuffer(*_data);
    _frameBuffer = apeFrameBuffer.getBuffer();

    if (_frameBuffer.empty())
    {
        return nullptr;
    }

    return _data;
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

std::shared_ptr<ZtaData> ZtaF::data()
{
    return _data;
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