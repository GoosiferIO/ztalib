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
        std::vector<uint8_t> pixels; // continuous array of pixels: i.e. {0,0,0,255,255,255,255,...}
        int width;
        int height;
        int offsetX;
        int offsetY;
        int channels;
    };
    ApeFrameBuffer(const ApeData& data);
    ~ApeFrameBuffer();
    const std::vector<BufferObject>& getBuffer();

private:
    int createBuffer();
    const ApeData& _data;
    int _colorModel = 0; // 0 = RGBA, 1 = BGRA
    std::vector<BufferObject> _buffer;
};

#endif // APEFRAMEBUFFER_H
