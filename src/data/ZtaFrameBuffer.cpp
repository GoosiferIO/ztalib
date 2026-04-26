#include "ztalib/data/ZtaFrameBuffer.h"

/* ZtaFrameBuffer.cpp -- buffer with several frames

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

ZtaFrameBuffer::ZtaFrameBuffer(const ZtaData& data)
    : m_data(data),
      m_colorModel(0)
{
    m_buffer = std::vector<ZtaFrameBufferObject>();
    createBuffer();
}

ZtaFrameBuffer::~ZtaFrameBuffer()
{
    m_buffer.clear();
}

const std::vector<ZtaFrameBufferObject>& ZtaFrameBuffer::getBuffer()
{
    return m_buffer;
}

int ZtaFrameBuffer::createBuffer()
{
    if (m_data.frames.empty())
    {
        return 0;
    }

    if (m_data.palette == nullptr || m_data.palette->colors().size() == 0)
    {
        std::cerr << "ERROR: No palette data available!" << std::endl;
        return -1;
    }

    for (const ZtaFrame &frame : m_data.frames)
    {
        ZtaFrameBufferObject bufferObject;

        // Set dimensions and format
        bufferObject.width = static_cast<int>(frame.width);
        bufferObject.height = static_cast<int>(frame.height);
        bufferObject.offsetX = static_cast<int>(frame.x);
        bufferObject.offsetY = static_cast<int>(frame.y);
        bufferObject.channels = 4; // RGBA/BGRA

        // Calculate m_buffer size and initialize with transparent pixels
        size_t bufferSize = bufferObject.width 
            * bufferObject.height 
            * bufferObject.channels;
        bufferObject.pixels = std::vector<uint8_t>(bufferSize);
        for (size_t i = 0; i < bufferSize; i += 4)
        {
            bufferObject.pixels[i] = 0;     // R
            bufferObject.pixels[i + 1] = 0; // G
            bufferObject.pixels[i + 2] = 0; // B
            bufferObject.pixels[i + 3] = 0; // A (Fully transparent)
        }

        // Process each row
        for (int row = 0; row < frame.height; row++)
        {
            if (row >= (int)frame.pixelSets.size())
            {
                std::cerr << "ERROR: Row " << row << " exceeds ZtaPixelSet count!" << std::endl;
                continue;
            }

            ZtaPixelSet pixelSet = frame.pixelSets[row];
            int xPos = 0; // Reset horizontal position for each new row

            // Process each block in the row
            for (const ZtaPixelBlock &block : pixelSet.blocks)
            {
                // Apply offset from current position
                xPos += block.offset;

                // Skip if this is an end-of-line block or empty block
                if (block.colorCount == 0)
                {
                    continue;
                }

                // Process each color in the block
                for (uint8_t colorIndex : block.colors)
                {
                    // Bounds checking
                    if (xPos >= bufferObject.width)
                    {
                        break; // Stop if we've exceeded the width
                    }

                    // Validate color index
                    if (colorIndex >= m_data.palette->colors().size())
                    {
                        std::cerr << "ERROR: Out-of-bounds color index! (" << (int)colorIndex << ")" << std::endl;
                        break;
                    }

                    // Calculate pixel position in m_buffer
                    size_t pixelIndex = (row * bufferObject.width + xPos) * bufferObject.channels;

                    // Ensure we don't write outside the m_buffer
                    if (pixelIndex + 3 >= bufferSize)
                    {
                        std::cerr << "ERROR: Buffer overflow prevented at position "
                                  << pixelIndex << std::endl;
                        break;
                    }

                    // Get color from palette
                    PalF::Color color;

                    try {
                        color = m_data.palette->getColor(colorIndex);
                    }
                    catch (const std::out_of_range &e)
                    {
                        std::cerr << "ERROR: " << e.what() << " Color index: " << (int)colorIndex << std::endl;
                        break;
                    }

                    // Write pixel data according to color model
                    if (m_colorModel == 1)
                    { // BGRA mode
                        bufferObject.pixels[pixelIndex] = color.b;
                        bufferObject.pixels[pixelIndex + 1] = color.g;
                        bufferObject.pixels[pixelIndex + 2] = color.r;
                        bufferObject.pixels[pixelIndex + 3] = color.a;
                    }
                    else
                    { // RGBA mode
                        bufferObject.pixels[pixelIndex] = color.r;
                        bufferObject.pixels[pixelIndex + 1] = color.g;
                        bufferObject.pixels[pixelIndex + 2] = color.b;
                        bufferObject.pixels[pixelIndex + 3] = color.a;
                    }

                    xPos++; // Move to next horizontal position
                }
            }
        }

        // Store the completed m_buffer
        m_buffer.push_back(bufferObject);
    }

    return 1;
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
