#ifndef SHIM_SCEGXM_H
#define SHIM_SCEGXM_H

/*
 * shim_SceGxm.h
 * PS Vita SceGxm → WebGL 2.0 shim
 *
 * GXM is the Vita's proprietary GPU API (roughly OpenGL ES 2.0 subset +
 * proprietary memory management and render-target model).
 * This shim translates the minimal GXM surface used by Minecraft Vita to
 * WebGL 2.0 (with ES 2.0 fallback via USE_WEBGL2=1 / FULL_ES3=1).
 *
 * Scope:
 *   - Context initialisation (sceGxmInitialize / sceGxmTerminate)
 *   - Render-target creation / deletion
 *   - Vertex/fragment shader pair → WebGL program
 *   - Vertex stream + uniform buffer → WebGL VAO + UBO
 *   - Draw calls → gl.drawArrays / gl.drawElements
 *   - Texture operations → WebGL texture objects
 *   - Sync / fence primitives (stubbed)
 *
 * ABI reference: vitasdk <gxm.h>
 *
 * Build requirements:
 *   -s USE_WEBGL2=1
 *   -s FULL_ES3=1
 *   --js-library shim_SceGxm.js
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Return codes ---------- */
#ifndef SCE_OK
#define SCE_OK 0
#endif
#define SCE_GXM_ERROR_INVALID_POINTER     ((int)0x805B0000)
#define SCE_GXM_ERROR_INVALID_VALUE       ((int)0x805B0001)
#define SCE_GXM_ERROR_INVALID_ALIGNMENT   ((int)0x805B0002)
#define SCE_GXM_ERROR_OUT_OF_MEMORY       ((int)0x805B0005)
#define SCE_GXM_ERROR_UNSUPPORTED         ((int)0x805B000E)

/* ---------- Primitive types ---------- */
typedef void SceGxmContext;
typedef void SceGxmRenderTarget;
typedef void SceGxmShaderPatcher;
typedef void SceGxmVertexProgram;
typedef void SceGxmFragmentProgram;
typedef void SceGxmSyncObject;

/* Texture opaque handle */
typedef struct { uint32_t data[16]; } SceGxmTexture;

/* ---------- Texture formats (subset) ---------- */
#define SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR  0x94000000
#define SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ARGB  0x90000000
#define SCE_GXM_TEXTURE_FORMAT_U8_R            0x60000000

/* ---------- Texture types ---------- */
#define SCE_GXM_TEXTURE_LINEAR      0
#define SCE_GXM_TEXTURE_SWIZZLED    1
#define SCE_GXM_TEXTURE_CUBE        2
#define SCE_GXM_TEXTURE_LINEAR_STRIDED 3

/* ---------- Texture filter ---------- */
#define SCE_GXM_TEXTURE_FILTER_POINT        0
#define SCE_GXM_TEXTURE_FILTER_LINEAR       1
#define SCE_GXM_TEXTURE_FILTER_MIPMAP_POINT 2

/* ---------- Texture address mode ---------- */
#define SCE_GXM_TEXTURE_ADDR_REPEAT         0
#define SCE_GXM_TEXTURE_ADDR_MIRROR         1
#define SCE_GXM_TEXTURE_ADDR_CLAMP          2

/* ---------- Primitive type ---------- */
typedef enum {
    SCE_GXM_PRIMITIVE_TRIANGLES = 0,
    SCE_GXM_PRIMITIVE_TRIANGLE_STRIP = 1,
    SCE_GXM_PRIMITIVE_TRIANGLE_FAN   = 2,
    SCE_GXM_PRIMITIVE_LINES          = 3,
    SCE_GXM_PRIMITIVE_POINTS         = 4
} SceGxmPrimitiveType;

/* ---------- Index format ---------- */
typedef enum {
    SCE_GXM_INDEX_FORMAT_U16 = 0,
    SCE_GXM_INDEX_FORMAT_U32 = 1
} SceGxmIndexFormat;

/* ---------- Colour surface format ---------- */
#define SCE_GXM_COLOR_FORMAT_A8B8G8R8 0x90080000

/* ---------- Colour surface type ---------- */
#define SCE_GXM_COLOR_SURFACE_LINEAR    0
#define SCE_GXM_COLOR_SURFACE_TILED     1
#define SCE_GXM_COLOR_SURFACE_SWIZZLED  2

/* ---------- Depth/stencil surface format ---------- */
#define SCE_GXM_DEPTH_STENCIL_FORMAT_DF32   0x00002000
#define SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24  0x00006000

/* ---------- Structs ---------- */
typedef struct SceGxmInitializeParams {
    uint32_t flags;
    uint32_t displayQueueMaxPendingCount;
    void    *displayQueueCallback;
    uint32_t displayQueueCallbackDataSize;
    size_t   parameterBufferSize;
} SceGxmInitializeParams;

typedef struct SceGxmContextParams {
    void    *hostMem;
    size_t   hostMemSize;
    void    *vdmRingBufferMem;
    size_t   vdmRingBufferMemSize;
    void    *vertexRingBufferMem;
    size_t   vertexRingBufferMemSize;
    void    *fragmentRingBufferMem;
    size_t   fragmentRingBufferMemSize;
    void    *fragmentUsseRingBufferMem;
    size_t   fragmentUsseRingBufferMemSize;
    uint32_t fragmentUsseRingBufferOffset;
} SceGxmContextParams;

typedef struct SceGxmRenderTargetParams {
    uint32_t flags;
    uint16_t width, height;
    uint32_t scenesPerFrame;
    uint32_t multisampleMode;
    uint32_t multisampleLocations;
    void    *driverMemBlock;
} SceGxmRenderTargetParams;

typedef struct SceGxmColorSurface {
    uint32_t pbeSideband;
    uint32_t pbeEmit;
    uint32_t outputRegion;
    uint32_t strideInPixels;
    void    *data;
    uint32_t colorFormat;
    uint32_t surfaceType;
    uint32_t gammaMode;
    uint32_t scaleMode;
} SceGxmColorSurface;

typedef struct SceGxmDepthStencilSurface {
    uint32_t zlsControl;
    void    *depthData;
    void    *stencilData;
    float    backgroundDepth;
    uint32_t backgroundControl;
} SceGxmDepthStencilSurface;

/* ---------- Minimal API surface ---------- */
int sceGxmInitialize(const SceGxmInitializeParams *params);
int sceGxmTerminate(void);

int sceGxmCreateContext(const SceGxmContextParams *params,
                        SceGxmContext **context);
int sceGxmDestroyContext(SceGxmContext *context);

int sceGxmCreateRenderTarget(const SceGxmRenderTargetParams *params,
                              SceGxmRenderTarget **renderTarget);
int sceGxmDestroyRenderTarget(SceGxmRenderTarget *renderTarget);

int sceGxmColorSurfaceInit(SceGxmColorSurface *surface, uint32_t colorFormat,
                            uint32_t surfaceType, uint32_t outputRegion,
                            uint32_t strideInPixels, void *data);
int sceGxmDepthStencilSurfaceInit(SceGxmDepthStencilSurface *surface,
                                   uint32_t depthStencilFormat,
                                   uint32_t depthStencilSurfaceType,
                                   uint32_t strideInSamples,
                                   void *depthData, void *stencilData);

int sceGxmBeginScene(SceGxmContext *context, uint32_t flags,
                      const SceGxmRenderTarget *renderTarget,
                      const void *validRegion,
                      SceGxmSyncObject *vertexSyncObject,
                      SceGxmSyncObject *fragmentSyncObject,
                      const SceGxmColorSurface *colorSurface,
                      const SceGxmDepthStencilSurface *depthStencilSurface);
int sceGxmEndScene(SceGxmContext *context,
                   const void *vertexSyncObject,
                   const void *fragmentSyncObject);
int sceGxmFinish(SceGxmContext *context);

/* Texture API */
int sceGxmTextureInitLinear(SceGxmTexture *tex, const void *data,
                             uint32_t texFormat, uint32_t width,
                             uint32_t height, uint32_t mipCount);
int sceGxmTextureSetMinFilter(SceGxmTexture *tex, uint32_t minFilter);
int sceGxmTextureSetMagFilter(SceGxmTexture *tex, uint32_t magFilter);
int sceGxmTextureSetUAddrMode(SceGxmTexture *tex, uint32_t addrMode);
int sceGxmTextureSetVAddrMode(SceGxmTexture *tex, uint32_t addrMode);
uint32_t sceGxmTextureGetWidth(const SceGxmTexture *tex);
uint32_t sceGxmTextureGetHeight(const SceGxmTexture *tex);

/* Sync objects (stubbed) */
int sceGxmSyncObjectCreate(SceGxmSyncObject **syncObject);
int sceGxmSyncObjectDestroy(SceGxmSyncObject *syncObject);
int sceGxmDisplayQueueAddEntry(SceGxmSyncObject *oldBuffer,
                                SceGxmSyncObject *newBuffer,
                                const void *callbackData);

/* Draw calls */
int sceGxmDraw(SceGxmContext *context, SceGxmPrimitiveType primType,
               SceGxmIndexFormat indexType, const void *indexData,
               uint32_t indexCount);
int sceGxmDrawInstanced(SceGxmContext *context, SceGxmPrimitiveType primType,
                         SceGxmIndexFormat indexType, const void *indexData,
                         uint32_t indexCount, uint32_t indexWrap);

#ifdef __cplusplus
}
#endif

#endif /* SHIM_SCEGXM_H */
