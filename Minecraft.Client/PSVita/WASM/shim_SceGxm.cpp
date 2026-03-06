/*
 * shim_SceGxm.cpp
 * PS Vita SceGxm → WebGL 2.0 shim implementation
 *
 * Build requirements:
 *   -s USE_WEBGL2=1
 *   -s FULL_ES3=1
 *   --js-library shim_SceGxm.js
 *
 * Design:
 *   Most GXM state is forwarded to WebGL via the JS glue layer
 *   (vita_gxm_*) because WebGL calls must happen on the main browser thread.
 *   Only book-keeping (handle allocation, parameter validation) lives here in C.
 *
 * Thread safety:
 *   In a pthreads build, WebGL calls MUST be issued from the main thread.
 *   Use emscripten_sync_run_in_main_runtime_thread or proxy queues for
 *   any render thread that is not the main thread.
 */

#include "shim_SceGxm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __EMSCRIPTEN__
#  include <emscripten.h>
#  include <emscripten/html5_webgl.h>
#endif

/* =========================================================================
 * Opaque handle types (allocated on heap, pointer = handle)
 * ========================================================================= */

typedef struct {
    int dummy;
} GxmContextImpl;

typedef struct {
    uint16_t width, height;
} GxmRenderTargetImpl;

typedef struct {
    uint32_t glTexId;
    uint32_t width, height;
    uint32_t format;
    uint32_t minFilter, magFilter;
    uint32_t uAddr, vAddr;
} GxmTextureImpl;

typedef struct {
    int dummy;
} GxmSyncObjectImpl;

/* =========================================================================
 * Helper: map SceGxm texture format → GL internalFormat / format / type
 * ========================================================================= */

typedef struct { int internalFmt; int fmt; int type; } GlTexFmt;

static GlTexFmt gxm_to_gl_texfmt(uint32_t gxmFmt)
{
    switch (gxmFmt & 0xFF000000u) {
        case 0x94000000: /* U8U8U8U8_ABGR */ return (GlTexFmt){ 0x8058 /*RGBA8*/, 0x1908 /*RGBA*/, 0x1401 /*UNSIGNED_BYTE*/ };
        case 0x90000000: /* U8U8U8U8_ARGB */ return (GlTexFmt){ 0x8058, 0x1908, 0x1401 };
        case 0x60000000: /* U8_R          */ return (GlTexFmt){ 0x8229 /*R8*/,   0x1903 /*RED*/,  0x1401 };
        default:                             return (GlTexFmt){ 0x8058, 0x1908, 0x1401 };
    }
}

/* =========================================================================
 * sceGxmInitialize / sceGxmTerminate
 * VITA:  Sets up the GPU library (DMA rings, display callback, etc.)
 * WEB:   Creates a WebGL2 context on the canvas element via JS glue
 * ========================================================================= */

int sceGxmInitialize(const SceGxmInitializeParams *params)
{
    (void)params;
#ifdef __EMSCRIPTEN__
    int ok = EM_ASM_INT({ return vita_gxm_initialize(); });
    if (!ok) return SCE_GXM_ERROR_INVALID_VALUE;
#endif
    return SCE_OK;
}

int sceGxmTerminate(void)
{
#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_gxm_terminate(); });
#endif
    return SCE_OK;
}

/* =========================================================================
 * Context
 * ========================================================================= */

int sceGxmCreateContext(const SceGxmContextParams *params,
                        SceGxmContext **context)
{
    (void)params;
    if (!context) return SCE_GXM_ERROR_INVALID_POINTER;
    GxmContextImpl *ctx = (GxmContextImpl *)malloc(sizeof(GxmContextImpl));
    if (!ctx) return SCE_GXM_ERROR_OUT_OF_MEMORY;
    ctx->dummy = 0;
    *context = (SceGxmContext *)ctx;
    return SCE_OK;
}

int sceGxmDestroyContext(SceGxmContext *context)
{
    if (!context) return SCE_GXM_ERROR_INVALID_POINTER;
    free(context);
    return SCE_OK;
}

/* =========================================================================
 * Render target
 * ========================================================================= */

int sceGxmCreateRenderTarget(const SceGxmRenderTargetParams *params,
                              SceGxmRenderTarget **renderTarget)
{
    if (!params || !renderTarget) return SCE_GXM_ERROR_INVALID_POINTER;
    GxmRenderTargetImpl *rt =
        (GxmRenderTargetImpl *)malloc(sizeof(GxmRenderTargetImpl));
    if (!rt) return SCE_GXM_ERROR_OUT_OF_MEMORY;
    rt->width  = params->width;
    rt->height = params->height;
    *renderTarget = (SceGxmRenderTarget *)rt;
    return SCE_OK;
}

int sceGxmDestroyRenderTarget(SceGxmRenderTarget *renderTarget)
{
    if (!renderTarget) return SCE_GXM_ERROR_INVALID_POINTER;
    free(renderTarget);
    return SCE_OK;
}

/* =========================================================================
 * Surface initialisation (book-keeping only; no GL calls needed at this stage)
 * ========================================================================= */

int sceGxmColorSurfaceInit(SceGxmColorSurface *surface, uint32_t colorFormat,
                            uint32_t surfaceType, uint32_t outputRegion,
                            uint32_t strideInPixels, void *data)
{
    if (!surface) return SCE_GXM_ERROR_INVALID_POINTER;
    memset(surface, 0, sizeof(*surface));
    surface->colorFormat    = colorFormat;
    surface->surfaceType    = surfaceType;
    surface->strideInPixels = strideInPixels;
    surface->data           = data;
    surface->outputRegion   = outputRegion;
    return SCE_OK;
}

int sceGxmDepthStencilSurfaceInit(SceGxmDepthStencilSurface *surface,
                                   uint32_t depthStencilFormat,
                                   uint32_t depthStencilSurfaceType,
                                   uint32_t strideInSamples,
                                   void *depthData, void *stencilData)
{
    (void)depthStencilSurfaceType; (void)strideInSamples;
    if (!surface) return SCE_GXM_ERROR_INVALID_POINTER;
    memset(surface, 0, sizeof(*surface));
    surface->depthData   = depthData;
    surface->stencilData = stencilData;
    surface->backgroundDepth = 1.0f;
    /* Store format in unused zlsControl field */
    surface->zlsControl = depthStencilFormat;
    return SCE_OK;
}

/* =========================================================================
 * Scene begin/end
 * VITA:  Marks the start/end of GPU rendering into a render target
 * WEB:   Binds the default framebuffer and clears; delegates to JS glue
 * ========================================================================= */

int sceGxmBeginScene(SceGxmContext *context, uint32_t flags,
                      const SceGxmRenderTarget *renderTarget,
                      const void *validRegion,
                      SceGxmSyncObject *vertexSyncObject,
                      SceGxmSyncObject *fragmentSyncObject,
                      const SceGxmColorSurface *colorSurface,
                      const SceGxmDepthStencilSurface *depthStencilSurface)
{
    (void)flags; (void)validRegion;
    (void)vertexSyncObject; (void)fragmentSyncObject;
    (void)colorSurface; (void)depthStencilSurface;
    if (!context) return SCE_GXM_ERROR_INVALID_POINTER;
#ifdef __EMSCRIPTEN__
    GxmRenderTargetImpl *rt = (GxmRenderTargetImpl *)renderTarget;
    EM_ASM({
        vita_gxm_begin_scene($0, $1);
    }, rt ? rt->width : 960, rt ? rt->height : 544);
#endif
    return SCE_OK;
}

int sceGxmEndScene(SceGxmContext *context,
                   const void *vertexSyncObject,
                   const void *fragmentSyncObject)
{
    (void)vertexSyncObject; (void)fragmentSyncObject;
    if (!context) return SCE_GXM_ERROR_INVALID_POINTER;
#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_gxm_end_scene(); });
#endif
    return SCE_OK;
}

int sceGxmFinish(SceGxmContext *context)
{
    (void)context;
    /* gl.finish() equivalent — flush all pending GL commands */
#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_gxm_finish(); });
#endif
    return SCE_OK;
}

/* =========================================================================
 * Texture API
 * VITA:  sceGxmTextureInitLinear creates a texture from a pixel buffer
 * WEB:   Creates a WebGL texture object and uploads the data via JS glue
 * ========================================================================= */

int sceGxmTextureInitLinear(SceGxmTexture *tex, const void *data,
                             uint32_t texFormat, uint32_t width,
                             uint32_t height, uint32_t mipCount)
{
    (void)mipCount;
    if (!tex) return SCE_GXM_ERROR_INVALID_POINTER;
    GxmTextureImpl *impl = (GxmTextureImpl *)malloc(sizeof(GxmTextureImpl));
    if (!impl) return SCE_GXM_ERROR_OUT_OF_MEMORY;
    memset(impl, 0, sizeof(*impl));
    impl->width  = width;
    impl->height = height;
    impl->format = texFormat;
    impl->minFilter = SCE_GXM_TEXTURE_FILTER_LINEAR;
    impl->magFilter = SCE_GXM_TEXTURE_FILTER_LINEAR;
    impl->uAddr = SCE_GXM_TEXTURE_ADDR_REPEAT;
    impl->vAddr = SCE_GXM_TEXTURE_ADDR_REPEAT;

    GlTexFmt gf = gxm_to_gl_texfmt(texFormat);

#ifdef __EMSCRIPTEN__
    uint32_t glId = (uint32_t)EM_ASM_INT({
        return vita_gxm_texture_create($0, $1, $2, $3, $4, $5);
    }, data, (int)width, (int)height,
       gf.internalFmt, gf.fmt, gf.type);
    if (glId == 0) {
        free(impl);
        return SCE_GXM_ERROR_OUT_OF_MEMORY;
    }
    impl->glTexId = glId;
#endif

    /* Store impl pointer inside the opaque tex struct (first field) */
    memcpy(tex->data, &impl, sizeof(void *));
    return SCE_OK;
}

static GxmTextureImpl *get_tex_impl(const SceGxmTexture *tex)
{
    if (!tex) return NULL;
    GxmTextureImpl *impl = NULL;
    memcpy(&impl, tex->data, sizeof(void *));
    return impl;
}

int sceGxmTextureSetMinFilter(SceGxmTexture *tex, uint32_t minFilter)
{
    GxmTextureImpl *impl = get_tex_impl(tex);
    if (!impl) return SCE_GXM_ERROR_INVALID_POINTER;
    impl->minFilter = minFilter;
#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_gxm_texture_set_filter($0, $1, $2); },
           impl->glTexId,
           minFilter == SCE_GXM_TEXTURE_FILTER_POINT ? 0x2600 /*NEAREST*/ : 0x2601 /*LINEAR*/,
           0 /*min*/);
#endif
    return SCE_OK;
}

int sceGxmTextureSetMagFilter(SceGxmTexture *tex, uint32_t magFilter)
{
    GxmTextureImpl *impl = get_tex_impl(tex);
    if (!impl) return SCE_GXM_ERROR_INVALID_POINTER;
    impl->magFilter = magFilter;
#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_gxm_texture_set_filter($0, $1, $2); },
           impl->glTexId,
           magFilter == SCE_GXM_TEXTURE_FILTER_POINT ? 0x2600 : 0x2601,
           1 /*mag*/);
#endif
    return SCE_OK;
}

int sceGxmTextureSetUAddrMode(SceGxmTexture *tex, uint32_t addrMode)
{
    GxmTextureImpl *impl = get_tex_impl(tex);
    if (!impl) return SCE_GXM_ERROR_INVALID_POINTER;
    impl->uAddr = addrMode;
    return SCE_OK;
}

int sceGxmTextureSetVAddrMode(SceGxmTexture *tex, uint32_t addrMode)
{
    GxmTextureImpl *impl = get_tex_impl(tex);
    if (!impl) return SCE_GXM_ERROR_INVALID_POINTER;
    impl->vAddr = addrMode;
    return SCE_OK;
}

uint32_t sceGxmTextureGetWidth(const SceGxmTexture *tex)
{
    GxmTextureImpl *impl = get_tex_impl(tex);
    return impl ? impl->width : 0;
}

uint32_t sceGxmTextureGetHeight(const SceGxmTexture *tex)
{
    GxmTextureImpl *impl = get_tex_impl(tex);
    return impl ? impl->height : 0;
}

/* =========================================================================
 * Sync objects (stubbed)
 * VITA:  GPU fence objects used to synchronise CPU/GPU and double buffering
 * WEB:   WebGL has no explicit fences in GL ES 2/3; gl.finish() is sufficient
 * NOTE:  [VITA_SHIM_STUB]
 * ========================================================================= */

int sceGxmSyncObjectCreate(SceGxmSyncObject **syncObject)
{
    fprintf(stderr,
        "[VITA_SHIM_STUB] sceGxmSyncObjectCreate: sync objects unsupported "
        "in WASM; returning a dummy handle\n");
    if (!syncObject) return SCE_GXM_ERROR_INVALID_POINTER;
    *syncObject = (SceGxmSyncObject *)malloc(sizeof(GxmSyncObjectImpl));
    return (*syncObject) ? SCE_OK : SCE_GXM_ERROR_OUT_OF_MEMORY;
}

int sceGxmSyncObjectDestroy(SceGxmSyncObject *syncObject)
{
    if (!syncObject) return SCE_GXM_ERROR_INVALID_POINTER;
    free(syncObject);
    return SCE_OK;
}

int sceGxmDisplayQueueAddEntry(SceGxmSyncObject *oldBuffer,
                                SceGxmSyncObject *newBuffer,
                                const void *callbackData)
{
    (void)oldBuffer; (void)newBuffer;
    fprintf(stderr,
        "[VITA_SHIM_STUB] sceGxmDisplayQueueAddEntry: "
        "display queue is a no-op in WASM\n");
    /* Invoke the display callback from sceGxmInitialize if needed */
    (void)callbackData;
    return SCE_OK;
}

/* =========================================================================
 * Draw calls
 * VITA:  sceGxmDraw
 * WEB:   gl.drawElements / gl.drawArrays via JS glue
 * NOTE:  Vertex/index buffers must already be bound by the caller.
 * ========================================================================= */

static int gxm_prim_to_gl(SceGxmPrimitiveType prim)
{
    switch (prim) {
        case SCE_GXM_PRIMITIVE_TRIANGLES:      return 0x0004; /* GL_TRIANGLES */
        case SCE_GXM_PRIMITIVE_TRIANGLE_STRIP: return 0x0005; /* GL_TRIANGLE_STRIP */
        case SCE_GXM_PRIMITIVE_TRIANGLE_FAN:   return 0x0006; /* GL_TRIANGLE_FAN */
        case SCE_GXM_PRIMITIVE_LINES:          return 0x0001; /* GL_LINES */
        case SCE_GXM_PRIMITIVE_POINTS:         return 0x0000; /* GL_POINTS */
        default:                               return 0x0004;
    }
}

int sceGxmDraw(SceGxmContext *context, SceGxmPrimitiveType primType,
               SceGxmIndexFormat indexType, const void *indexData,
               uint32_t indexCount)
{
    if (!context) return SCE_GXM_ERROR_INVALID_POINTER;
    int glPrim  = gxm_prim_to_gl(primType);
    int glType  = (indexType == SCE_GXM_INDEX_FORMAT_U16) ? 0x1403 /*UNSIGNED_SHORT*/
                                                           : 0x1405 /*UNSIGNED_INT*/;
#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_gxm_draw($0, $1, $2, $3); },
           glPrim, glType, indexData, (int)indexCount);
#endif
    return SCE_OK;
}

int sceGxmDrawInstanced(SceGxmContext *context, SceGxmPrimitiveType primType,
                         SceGxmIndexFormat indexType, const void *indexData,
                         uint32_t indexCount, uint32_t indexWrap)
{
    (void)indexWrap;
    return sceGxmDraw(context, primType, indexType, indexData, indexCount);
}
