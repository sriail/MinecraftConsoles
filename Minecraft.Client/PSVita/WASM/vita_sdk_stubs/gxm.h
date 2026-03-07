#pragma once
#include <stdint.h>
#include <stddef.h>

#ifndef SCE_OK
#define SCE_OK 0
#endif

typedef struct SceGxmTexture { uint32_t data[16]; } SceGxmTexture;
typedef struct SceGxmColorSurface { uint32_t data[8]; } SceGxmColorSurface;
typedef struct SceGxmDepthStencilSurface { uint32_t data[8]; } SceGxmDepthStencilSurface;

typedef struct SceGxmRenderTarget SceGxmRenderTarget;
typedef struct SceGxmContext SceGxmContext;
typedef struct SceGxmShaderPatcher SceGxmShaderPatcher;
typedef struct SceGxmVertexProgram SceGxmVertexProgram;
typedef struct SceGxmFragmentProgram SceGxmFragmentProgram;
typedef struct SceGxmProgram SceGxmProgram;
typedef struct SceGxmSyncObject SceGxmSyncObject;

typedef uint32_t SceGxmShaderPatcherId;

typedef struct SceGxmInitializeParams {
    uint32_t flags;
    uint32_t displayQueueMaxPendingCount;
    void    *displayQueueCallback;
    uint32_t displayQueueCallbackDataSize;
    uint32_t parameterBufferSize;
} SceGxmInitializeParams;

typedef struct SceGxmContextParams {
    void    *hostMem;
    uint32_t hostMemSize;
    void    *vdmRingBufferMem;
    uint32_t vdmRingBufferMemSize;
    void    *vertexRingBufferMem;
    uint32_t vertexRingBufferMemSize;
    void    *fragmentRingBufferMem;
    uint32_t fragmentRingBufferMemSize;
    void    *fragmentUsseRingBufferMem;
    uint32_t fragmentUsseRingBufferMemSize;
    uint32_t fragmentUsseRingBufferOffset;
} SceGxmContextParams;

typedef struct SceGxmRenderTargetParams {
    uint32_t flags;
    uint16_t width;
    uint16_t height;
    uint16_t scenesPerFrame;
    uint16_t multisampleMode;
    uint32_t multisampleLocations;
    void    *driverMemBlock;
} SceGxmRenderTargetParams;

typedef struct SceGxmVertexAttribute {
    uint16_t streamIndex;
    uint16_t offset;
    uint16_t format;
    uint16_t componentCount;
    uint16_t regIndex;
} SceGxmVertexAttribute;

typedef struct SceGxmVertexStream {
    uint16_t stride;
    uint16_t indexSource;
} SceGxmVertexStream;

typedef struct SceGxmBlendInfo {
    uint8_t colorMask;
    uint8_t colorFunc;
    uint8_t alphaFunc;
    uint8_t colorSrc;
    uint8_t colorDst;
    uint8_t alphaSrc;
    uint8_t alphaDst;
} SceGxmBlendInfo;

typedef struct SceGxmNotification {
    volatile uint32_t *address;
    uint32_t value;
} SceGxmNotification;

typedef struct SceGxmPrecomputedDraw { uint32_t data[16]; } SceGxmPrecomputedDraw;

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

typedef enum SceGxmPrimitiveType {
    SCE_GXM_PRIMITIVE_TRIANGLES      = 0,
    SCE_GXM_PRIMITIVE_TRIANGLE_STRIP = 1,
    SCE_GXM_PRIMITIVE_TRIANGLE_FAN   = 2,
    SCE_GXM_PRIMITIVE_LINES          = 3,
    SCE_GXM_PRIMITIVE_POINTS         = 4
} SceGxmPrimitiveType;

typedef enum SceGxmIndexFormat {
    SCE_GXM_INDEX_FORMAT_U16 = 0,
    SCE_GXM_INDEX_FORMAT_U32 = 1
} SceGxmIndexFormat;

typedef enum SceGxmContextType {
    SCE_GXM_CONTEXT_TYPE_IMMEDIATE = 0,
    SCE_GXM_CONTEXT_TYPE_DEFERRED  = 1
} SceGxmContextType;

typedef enum SceGxmOutputRegisterFormat {
    SCE_GXM_OUTPUT_REGISTER_FORMAT_DECLARED = 0
} SceGxmOutputRegisterFormat;

typedef enum SceGxmStencilFunc {
    SCE_GXM_STENCIL_FUNC_NEVER    = 0,
    SCE_GXM_STENCIL_FUNC_LESS     = 1,
    SCE_GXM_STENCIL_FUNC_EQUAL    = 2,
    SCE_GXM_STENCIL_FUNC_LEQUAL   = 3,
    SCE_GXM_STENCIL_FUNC_GREATER  = 4,
    SCE_GXM_STENCIL_FUNC_NOTEQUAL = 5,
    SCE_GXM_STENCIL_FUNC_GEQUAL   = 6,
    SCE_GXM_STENCIL_FUNC_ALWAYS   = 7
} SceGxmStencilFunc;

typedef enum SceGxmStencilOp {
    SCE_GXM_STENCIL_OP_KEEP    = 0,
    SCE_GXM_STENCIL_OP_ZERO    = 1,
    SCE_GXM_STENCIL_OP_REPLACE = 2,
    SCE_GXM_STENCIL_OP_INCR    = 3,
    SCE_GXM_STENCIL_OP_DECR    = 4,
    SCE_GXM_STENCIL_OP_INVERT  = 5,
    SCE_GXM_STENCIL_OP_INCR_WRAP = 6,
    SCE_GXM_STENCIL_OP_DECR_WRAP = 7
} SceGxmStencilOp;

typedef int SceGxmErrorCode;
