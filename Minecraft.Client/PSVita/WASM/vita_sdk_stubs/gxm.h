#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct SceGxmTexture { uint32_t data[16]; } SceGxmTexture;
typedef struct SceGxmColorSurface { uint32_t data[8]; } SceGxmColorSurface;
typedef struct SceGxmDepthStencilSurface { uint32_t data[8]; } SceGxmDepthStencilSurface;

typedef struct SceGxmRenderTarget SceGxmRenderTarget;
typedef struct SceGxmContext SceGxmContext;
typedef struct SceGxmShaderPatcher SceGxmShaderPatcher;
typedef struct SceGxmVertexProgram SceGxmVertexProgram;
typedef struct SceGxmFragmentProgram SceGxmFragmentProgram;

typedef enum SceGxmTextureFormat {
    SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR = 0x00000000
} SceGxmTextureFormat;

typedef enum SceGxmTextureFilter {
    SCE_GXM_TEXTURE_FILTER_POINT  = 0,
    SCE_GXM_TEXTURE_FILTER_LINEAR = 1
} SceGxmTextureFilter;

typedef enum SceGxmTextureAddrMode {
    SCE_GXM_TEXTURE_ADDR_REPEAT = 0,
    SCE_GXM_TEXTURE_ADDR_CLAMP  = 1
} SceGxmTextureAddrMode;

#define SCE_OK 0
