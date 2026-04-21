#ifndef APEFRAMEBUFFER_H
#define APEFRAMEBUFFER_H
#include <cstdint>
#include <memory>
#include "ApeData.h"

class ApeFrameBuffer
{
public:
    struct BufferObject
    {
        uint8_t *pixels; // continuous array of pixels: i.e. {0,0,0,255,255,255,255,...}
        int width;
        int height;
        int offsetX;
        int offsetY;
        int channels;
    };
    ApeFrameBuffer(const ApeData& data);
    ~ApeFrameBuffer();
    std::vector<BufferObject> getBuffer();

private:
    int createBuffer();
    const ApeData& _data;
    int _colorModel = 0; // 0 = RGBA, 1 = BGRA
    std::vector<BufferObject> _buffer;
};

ApeFrameBuffer::ApeFrameBuffer(const ApeData& data)
    : _data(data),
      _colorModel(0)
{
    _buffer = std::vector<BufferObject>();
    createBuffer();
}

std::vector<BufferObject> ApeFrameBuffer::getBuffer()
{
    return _buffer;
}

int ApeFrameBuffer::createBuffer()
{
    if (_data->frames.empty())
    {
        return 0;
    }

    int numBuffers = _data->info->frameCount;

    for (const ApeFrame &frame : _data->frames)
    {
        int index = &frame - &_data->frames[0];
        ApeFrameBuffer::BufferObject bufferObject 
        = ApeFrameBuffer::BufferObject();

        // Set dimensions and format
        bufferObject.width = static_cast<int>(frame->width);
        bufferObject.height = static_cast<int>(frame->height);
        bufferObject.offsetX = static_cast<int>(frame->x);
        bufferObject.offsetY = static_cast<int>(frame->y);
        bufferObject.channels = 4; // RGBA/BGRA

        // Calculate _buffer size and initialize with transparent pixels
        size_t bufferSize = bufferObject->width 
            * bufferObject->height 
            * bufferObject->channels;
        bufferObject.pixels = new uint8_t[bufferSize];
        for (size_t i = 0; i < bufferSize; i += 4)
        {
            bufferObject.pixels[i] = 0;     // R
            bufferObject.pixels[i + 1] = 0; // G
            bufferObject.pixels[i + 2] = 0; // B
            bufferObject.pixels[i + 3] = 0; // A (Fully transparent)
        }

        // Process each row
        for (int row = 0; row < frame->height; row++)
        {
            if (row >= frame.pixelSets.size())
            {
                std::cerr << "ERROR: Row " << row << " exceeds ApePixelSet count!" << std::endl;
                continue;
            }

            std::unique_ptr<ApePixelSet> pixelSet = std::make_unique<ApePixelSet>(frame.pixelSets[row]);
            int xPos = 0; // Reset horizontal position for each new row

            // Process each block in the row
            for (std::unique_ptr<ApePixelBlock> &block : pixelSet->blocks)
            {
                // Apply offset from current position
                xPos += block->offset;

                // Skip if this is an end-of-line block or empty block
                if (block->colorCount == 0)
                {
                    continue;
                }

                // Process each color in the block
                for (uint8_t colorIndex : block->colors)
                {
                    // Bounds checking
                    if (xPos >= bufferObject->width)
                    {
                        break; // Stop if we've exceeded the width
                    }

                    // Validate color index
                    if (colorIndex >= _data->palette->numColors)
                    {
                        std::cerr << "ERROR: Out-of-bounds color index! ("
                                  << (int)colorIndex << ")" << std::endl;
                        continue;
                    }

                    // Calculate pixel position in _buffer
                    size_t pixelIndex = (row * bufferObject->width + xPos) * bufferObject->channels;

                    // Ensure we don't write outside the _buffer
                    if (pixelIndex + 3 >= bufferSize)
                    {
                        std::cerr << "ERROR: Buffer overflow prevented at position "
                                  << pixelIndex << std::endl;
                        break;
                    }

                    // Get color from palette
                    std::unique_ptr<ApeColor> color = _data->palette->getColor(colorIndex);

                    // Write pixel data according to color model
                    if (_colorModel == 1)
                    { // BGRA mode
                        bufferObject.pixels[pixelIndex] = color->b;
                        bufferObject.pixels[pixelIndex + 1] = color->g;
                        bufferObject.pixels[pixelIndex + 2] = color->r;
                        bufferObject.pixels[pixelIndex + 3] = color->a;
                    }
                    else
                    { // RGBA mode
                        bufferObject.pixels[pixelIndex] = color->r;
                        bufferObject.pixels[pixelIndex + 1] = color->g;
                        bufferObject.pixels[pixelIndex + 2] = color->b;
                        bufferObject.pixels[pixelIndex + 3] = color->a;
                    }

                    xPos++; // Move to next horizontal position
                }
            }
        }

        // Store the completed _buffer
        _buffer.push_back(std::move(bufferObject));
    }

    return 1;
}

#endif // APEFRAMEBUFFER_H