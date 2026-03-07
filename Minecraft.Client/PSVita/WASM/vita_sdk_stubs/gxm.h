#pragma once
// Minimal PS Vita gxm.h stub for Emscripten/WASM builds.
// Only the types/constants referenced by this codebase are provided.

#include <stdint.h>
#include <stddef.h>

typedef struct SceGxmContext         SceGxmContext;
typedef struct SceGxmShaderPatcher   SceGxmShaderPatcher;
typedef struct SceGxmTexture         SceGxmTexture;
typedef struct SceGxmRenderTarget    SceGxmRenderTarget;
typedef struct SceGxmSyncObject      SceGxmSyncObject;
typedef struct SceGxmVertexProgram   SceGxmVertexProgram;
typedef struct SceGxmFragmentProgram SceGxmFragmentProgram;
typedef struct SceGxmVertexStream    SceGxmVertexStream;
typedef struct SceGxmVertexAttribute SceGxmVertexAttribute;
typedef struct SceGxmProgram         SceGxmProgram;
typedef struct SceGxmProgramParameter SceGxmProgramParameter;
typedef struct SceGxmColorSurface    SceGxmColorSurface;
typedef struct SceGxmDepthStencilSurface SceGxmDepthStencilSurface;

typedef int SceGxmErrorCode;
typedef uint32_t SceGxmTextureFormat;
typedef uint32_t SceGxmColorFormat;
typedef uint32_t SceGxmDepthStencilFormat;
typedef uint32_t SceGxmBlendFunc;
typedef uint32_t SceGxmBlendFactor;
typedef uint32_t SceGxmPrimitiveType;
typedef uint32_t SceGxmIndexFormat;

typedef struct SceGxmInitializeParams {
    uint32_t flags;
    uint32_t displayQueueMaxPendingCount;
    void     (*displayQueue)(const void *callbackData);
    uint32_t displayQueueCallbackDataSize;
    size_t   parameterBufferSize;
} SceGxmInitializeParams;

#define SCE_GXM_ERROR_UNINITIALIZED (-1)

#ifdef __cplusplus
extern "C" {
#endif

int  sceGxmInitialize(const SceGxmInitializeParams *params);
int  sceGxmTerminate(void);
void sceGxmFinish(SceGxmContext *context);

#ifdef __cplusplus
}
#endif
