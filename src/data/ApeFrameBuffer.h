#ifndef APEFRAMEBUFFER_H
#define APEFRAMEBUFFER_H
#include <cstdint>
#include <memory>
#include "ApeData.h"

class ApeFrameBuffer 
{
    public:
    struct ApeFrameBufferObject
    {
        uint8_t* pixels; // continuous array of pixels: i.e. {0,0,0,255,255,255,255,...}
        int width;
        int height;
        int offsetX;
        int offsetY;
        int channels;
    };
    ApeFrameBuffer(std::unique_ptr<ApeData> data);
    ~ApeFrameBuffer();
    std::vector<std::unique_ptr<ApeFrameBufferObject>> _buffer;
    std::vector<std::unique_ptr<ApeFrame>> _frames;
    private:
    int createBuffer();
    std::unique_ptr<ApeData> _data;
    int _colorModel = 0; // 0 = RGBA, 1 = BGRA
};

ApeFrameBuffer::ApeFrameBuffer(std::unique_ptr<ApeData> data) 
: _data(std::move(data)),
  _colorModel(0)
{
    _buffer = std::vector<std::unique_ptr<ApeFrameBufferObject>>();
    _frames = std::vector<std::unique_ptr<ApeFrame>>();
}

int ApeFrameBuffer::createBuffer() 
{
    if (_frames.empty()) {
        return 0;
    }

    int numBuffers = _data->info->frameCount;

    for (const std::unique_ptr<ApeFrame>& frame : _frames) 
    {
        int index = &frame - &_frames[0];
        std::unique_ptr<ApeFrameBufferObject> bufferObject = std::make_unique<ApeFrameBufferObject>();

        // Set dimensions and format
        bufferObject->width = static_cast<int>(frame->width);
        bufferObject->height = static_cast<int>(frame->height);
        bufferObject->offsetX = static_cast<int>(frame->x);
        bufferObject->offsetY = static_cast<int>(frame->y);
        bufferObject->channels = 4;  // RGBA/BGRA

        // Calculate _buffer size and initialize with transparent pixels
        size_t bufferSize = bufferObject->width * bufferObject->height * bufferObject->channels;
        bufferObject->pixels = new uint8_t[bufferSize];
        for (size_t i = 0; i < bufferSize; i += 4) {
            bufferObject->pixels[i] = 0;     // R
            bufferObject->pixels[i + 1] = 0; // G
            bufferObject->pixels[i + 2] = 0; // B
            bufferObject->pixels[i + 3] = 0; // A (Fully transparent)
        }

        // Process each row
        for (int row = 0; row < frame->height; row++) 
        {
            if (row >= frame->pixelSets.size()) {
                std::cerr << "ERROR: Row " << row << " exceeds ApePixelSet count!" << std::endl;
                continue;
            }

            std::unique_ptr<ApePixelSet> pixelSet = std::make_unique<ApePixelSet>(frame->pixelSets[row]);
            int xPos = 0;  // Reset horizontal position for each new row

            // Process each block in the row
            for (ApePixelBlock &ApePixelBlock : pixelSet->blocks) 
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
                    if (xPos >= bufferObject->width) {
                        break;  // Stop if we've exceeded the width
                    }

                    // Validate color index
                    if (colorIndex >= _data->palette->numColors) {
                        std::cerr << "ERROR: Out-of-bounds color index! (" 
                                 << (int)colorIndex << ")" << std::endl;
                        continue;
                    }

                    // Calculate pixel position in _buffer
                    size_t pixelIndex = (row * bufferObject->width + xPos) * bufferObject->channels;
                    
                    // Ensure we don't write outside the _buffer
                    if (pixelIndex + 3 >= bufferSize) {
                        std::cerr << "ERROR: Buffer overflow prevented at position " 
                                 << pixelIndex << std::endl;
                        break;
                    }

                    // Get color from palette
                    ApeColor &color = _data->palette->colors[colorIndex];

                    // Write pixel data according to color model
                    if (_colorModel == 1) {  // BGRA mode
                        bufferObject->pixels[pixelIndex] = color.b;
                        bufferObject->pixels[pixelIndex + 1] = color.g;
                        bufferObject->pixels[pixelIndex + 2] = color.r;
                        bufferObject->pixels[pixelIndex + 3] = color.a;
                    } else {  // RGBA mode
                        bufferObject->pixels[pixelIndex] = color.r;
                        bufferObject->pixels[pixelIndex + 1] = color.g;
                        bufferObject->pixels[pixelIndex + 2] = color.b;
                        bufferObject->pixels[pixelIndex + 3] = color.a;
                    }
                    
                    xPos++;  // Move to next horizontal position
                }
            }
        }
        
        // Store the completed _buffer
        _buffer.push_back(std::move(bufferObject));
    }

    return 1;
}

#endif // APEFRAMEBUFFER_H