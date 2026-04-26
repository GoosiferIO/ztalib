#include "ztalib/ZtaF.h"

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
    m_data = std::make_shared<ZtaData>();
    m_colorModel = 0;
    m_frameBuffer = std::vector<ZtaFrameBufferObject>();
}

ZtaF::~ZtaF()
{
    // free frames
    m_data->frames.clear();
}

std::vector<ZtaFrameBufferObject> ZtaF::getFrameBuffer()
{
    return m_frameBuffer;
}

// ZtaColor model 0 = RGBA
// ZtaColor model 1 = BGRA
std::shared_ptr<ZtaData> ZtaF::load(std::string fileName, int colorModel, std::string ioPal)
{
    this->m_colorModel = colorModel;
    std::ifstream file;
    file.open(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    if (!file.is_open())
    {
        return nullptr;
    }

    // ------------------------------- read header
    // Note: if bg frame exists, not counted in frameCount
    // check if fatz
    if (hasMagic(file))
    {
        // skip 8 bytes
        file.seekg(8, std::ios::cur);
        // read 9th byte
        file.read((char *)&m_data->hasBackground, 1);
    } // else, not fatz (ztaf)

    file.read((char *)&m_data->info.speed, 4);                         // animation speed in ms

    uint32_t paletteNameSize = 0;
    file.read((char *)&paletteNameSize, 4);                  // size of palette name
    m_data->palette->locationSize(paletteNameSize);             // set palette location size

    std::string paletteName(paletteNameSize, '\0');
    file.read(paletteName.data(), m_data->palette->locationSize()); // read palette name
    m_data->palette->location(paletteName);

    file.read((char *)&m_data->info.frameCount, 4);                    // number of frames
    m_data->frames.resize(m_data->info.frameCount);                      // resize frames to frame count

    std::string paletteLocation;
    if (ioPal.empty()) {
        paletteLocation = std::string(m_data->palette->location().data());
    } 
    else {
        paletteLocation = ioPal;
    }
    m_data->palette->location(paletteLocation); // set palette location

    if (m_data->hasBackground)
    {
        m_data->info.frameCount += 1;
        m_data->frames.resize(m_data->info.frameCount);
    }

    // ------------------------------- read palette
    // load palette from location specified in zta file if ioPal not given
    if (ioPal.empty()) {
        std::filesystem::path ztaPath(fileName);
        std::filesystem::path palettePath(m_data->palette->location());
        std::filesystem::path resolvedPalettePath = resolvePalPath(ztaPath, palettePath);
        m_data->palette->load(resolvedPalettePath.string());
    } else {
        std::filesystem::path palettePath(ioPal);
        // if relative path, add current working directory        
        if (palettePath.is_relative()) {
            palettePath = std::filesystem::current_path() / palettePath;
        }
        m_data->palette->load(palettePath.string());
    }

    // ------------------------------- read frames
    for (int i = 0; i < (int)m_data->info.frameCount; i++)
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
        m_data->frames[i] = std::move(frame);
    }

    file.close();

    // write output buffer
    ZtaFrameBuffer apeFrameBuffer(*m_data);
    m_frameBuffer = apeFrameBuffer.getBuffer();

    if (m_frameBuffer.empty())
    {
        return nullptr;
    }

    return m_data;
}

void ZtaF::save(std::string fileName, std::string projectRoot, std::string palettePath)
{
    std::ofstream file;
    file.open(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::out));
    if (!file.is_open())
    {
        return;
    }

    // ensure that project root is absolute path (or convert to absolute path)
    std::filesystem::path projRootPath = std::filesystem::weakly_canonical(projectRoot);
    std::filesystem::path palPath = std::filesystem::weakly_canonical(palettePath);
    std::filesystem::path relPalettePath = std::filesystem::relative(palPath, projRootPath);

    m_ztaPath = fileName; // store path for future saves

    // -------------------------------- write header
    // if hasBackground is true, write FATZ magic and background frame flag
    if (m_data->hasBackground) // m_data->info.frameCount > 1)
    {
        file.write("FATZ", 4); // magic
        file.write("\0\0\0\0", 4); // reserved
        file.write((char *)&m_data->hasBackground, 1); // background frame flag
    }
    // } else {
    file.write((char *)&m_data->info.speed, 4); // animation speed in ms

    std::string paletteLocation = relPalettePath.generic_string();
    uint32_t paletteNameSize = static_cast<uint32_t>(paletteLocation.size() + 1); // +1 for null terminator
    file.write((char *)&paletteNameSize, 4); // size of palette name
    file.write(paletteLocation.c_str(), paletteNameSize); // palette name

    uint32_t frameCount = m_data->info.frameCount;
    if (m_data->hasBackground)
    {
        frameCount -= 1; // if background frame exists, not counted in frameCount
    }
    file.write((char *)&frameCount, 4); // number of frames

    // -------------------------------- write frames
    for (int i = 0; i < (int)m_data->info.frameCount; i++)
    {
        ZtaFrame &frame = m_data->frames[i];
        file.write((char *)&frame.frameSize, 4);
        file.write((char *)&frame.height, 2);
        file.write((char *)&frame.width, 2);
        file.write((char *)&frame.y, 2);
        file.write((char *)&frame.x, 2);

        frame.unk1 = 0; // set unknown byte 1 to 00
        frame.unk2 = 1; // set unknown byte 2 to 01
        file.write((char *)&frame.unk1, 1); // unknown byte 1
        file.write((char *)&frame.unk2, 1); // unknown byte 2

        // write pixel sets
        for (const ZtaPixelSet &pixelSet : frame.pixelSets)
        {
            file.write((char *)&pixelSet.blockCount, 1); // how many pixel blocks
            for (const ZtaPixelBlock &block : pixelSet.blocks)
            { // write each block
                file.write((char *)&block.offset, 1);                      // offset
                file.write((char *)&block.colorCount, 1);                  // color count
                file.write((char *)block.colors.data(), block.colorCount); // colors
            }
        }
    }

    // -------------------------------- write palette
    m_data->palette->save(palPath.string());
}

std::shared_ptr<ZtaData> ZtaF::data()
{
    return m_data;
}

int ZtaF::hasMagic(std::ifstream &_file)
{
    char magic[5] = {0};
    _file.read(magic, 4);

    // read at least 4 bytes
    // if less than 4 bytes, not FATZ
    if (_file.gcount() < 4)
    {
        _file.clear();
        _file.seekg(0, std::ios::beg);
        return 0;
    }

    // test for FATZ
    if (strcmp(magic, "FATZ") != 0)
    {
        _file.clear();
        _file.seekg(0, std::ios::beg);
        return 0;
    }

    // FATZ found
    _file.clear();
    _file.seekg(0, std::ios::beg);
    return 1;
}

std::filesystem::path ZtaF::resolvePalPath(
    const std::filesystem::path& ztaPath, 
    const std::filesystem::path& palettePath)
{
    auto dir = ztaPath.parent_path();

    while (!dir.empty())
    {
        auto candidate = dir / palettePath;
        if (std::filesystem::exists(candidate))
        {
            // note: canonical makes it so that if the candidate is a symlink, 
            // it resolves to the actual file path
            return std::filesystem::canonical(candidate);
        }
        auto parentDir = dir.parent_path();
        if (parentDir == dir) {
            break; // reached root directory
        }
        dir = dir.parent_path();
    }

    return std::filesystem::path(); 
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