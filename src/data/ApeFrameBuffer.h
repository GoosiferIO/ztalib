#ifndef APEFRAMEBUFFER_H
#define APEFRAMEBUFFER_H
#include <cstdint>
#include <memory>
#include "ApeFrame.h"

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
    ApeFrameBuffer();
    ~ApeFrameBuffer();
    std::vector<std::unique_ptr<ApeFrameBufferObject>> buffer;
    std::vector<std::unique_ptr<ApeFrame>> frames;
    private:
    int createBuffer();
};

ApeFrameBuffer::ApeFrameBuffer()
{
    buffer = std::vector<std::unique_ptr<ApeFrameBufferObject>>();
    frames = std::vector<std::unique_ptr<ApeFrame>>();
}

int ApeFrameBuffer::createBuffer() 
{
    if (frames.empty()) {
        return 0;
    }

    int numBuffers = getFrameCount();

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
        buffers[index] = new ApeFrameBuffer(output);
    }

    return 1;
}

#endif // APEFRAMEBUFFER_H