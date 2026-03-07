#pragma once
#include <stdint.h>

typedef struct SceDisplayFrameBuf {
    uint32_t  size;
    void     *base;
    uint32_t  pitch;
    uint32_t  pixelformat;
    uint32_t  width;
    uint32_t  height;
} SceDisplayFrameBuf;
