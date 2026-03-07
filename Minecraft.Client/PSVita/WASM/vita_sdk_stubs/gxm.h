#pragma once
/* gxm.h — PS Vita GXM graphics API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <stdlib.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Opaque context types ────────────────────────────────────── */
typedef struct SceGxmContext           SceGxmContext;
typedef struct SceGxmShaderPatcher     SceGxmShaderPatcher;
typedef struct SceGxmRenderTarget      SceGxmRenderTarget;
typedef struct SceGxmFragmentProgram   SceGxmFragmentProgram;
typedef struct SceGxmVertexProgram     SceGxmVertexProgram;
typedef struct SceGxmSyncObject        SceGxmSyncObject;

/* ── Enum stubs ─────────────────────────────────────────────── */
typedef int SceGxmContextType;
typedef int SceGxmColorFormat;
typedef int SceGxmDepthStencilFormat;
typedef int SceGxmMultisampleMode;
typedef int SceGxmTextureFormat;
typedef int SceGxmTextureType;
typedef int SceGxmBlendFunc;
typedef int SceGxmBlendFactor;
typedef int SceGxmColorMask;
typedef int SceGxmStencilFunc;
typedef int SceGxmStencilOp;
typedef int SceGxmDepthFunc;
typedef int SceGxmCullMode;
typedef int SceGxmOutputRegisterFormat;
typedef int SceGxmErrorCode;

/* Color formats */
#define SCE_GXM_COLOR_FORMAT_A8B8G8R8   0x9401
#define SCE_GXM_COLOR_FORMAT_R8G8B8A8   0x9400
#define SCE_GXM_MULTISAMPLE_NONE        0
#define SCE_GXM_MULTISAMPLE_2X          1
#define SCE_GXM_MULTISAMPLE_4X          2

/* Texture formats */
#define SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR   0x00000000
#define SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ARGB   0x01000000
#define SCE_GXM_TEXTURE_BASE_FORMAT_U8U8U8U8   0x00000000
#define SCE_GXM_TEXTURE_FORMAT_U8_R             0x00000000

/* Context types */
#define SCE_GXM_CONTEXT_TYPE_IMMEDIATE  0
#define SCE_GXM_CONTEXT_TYPE_DEFERRED   1

/* Transfer format/type */
#define SCE_GXM_TRANSFER_FORMAT_U8U8U8U8_ABGR  0
#define SCE_GXM_TRANSFER_FORMAT_RAW128          1
#define SCE_GXM_TRANSFER_LINEAR                 0
#define SCE_GXM_TRANSFER_TILED                  1
#define SCE_GXM_TRANSFER_SWIZZLED               2
#define SCE_GXM_TRANSFER_COLORKEY_NONE          0

/* Blend */
#define SCE_GXM_BLEND_FUNC_ADD              0
#define SCE_GXM_BLEND_FACTOR_ZERO           0
#define SCE_GXM_BLEND_FACTOR_ONE            1
#define SCE_GXM_BLEND_FACTOR_SRC_ALPHA      4
#define SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA 5
#define SCE_GXM_COLOR_MASK_ALL              0xF

/* Depth stencil format */
#define SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24  0x00000000

/* ── Struct stubs ───────────────────────────────────────────── */
typedef struct {
    volatile unsigned int *address;
    unsigned int           value;
} SceGxmNotification;

typedef struct {
    uint8_t data[64];
} SceGxmTexture;

typedef struct {
    uint8_t data[16];
} SceGxmColorSurface;

typedef struct {
    uint8_t data[16];
} SceGxmDepthStencilSurface;

typedef struct {
    uint8_t data[64];
} SceGxmPrecomputedDraw;

typedef struct {
    unsigned short streamIndex;
    unsigned short offset;
    int            format;
    unsigned char  componentCount;
    unsigned char  regIndex;
} SceGxmVertexAttribute;

typedef struct {
    unsigned int stride;
    int          indexSource;
} SceGxmVertexStream;

typedef struct {
    int     colorMask;
    int     colorFunc;
    int     alphaFunc;
    int     colorSrc;
    int     colorDst;
    int     alphaSrc;
    int     alphaDst;
} SceGxmBlendInfo;

typedef struct {
    uint32_t data[16];
} SceGxmProgram;

typedef uint32_t SceGxmShaderPatcherId;

/* ── No-op API ──────────────────────────────────────────────── */
static inline int sceGxmInitialize(const void *params) { (void)params; return SCE_OK; }
static inline int sceGxmTerminate(void) { return SCE_OK; }

static inline int sceGxmCreateContext(const void *params, SceGxmContext **ppContext)
{ (void)params; *ppContext = NULL; return SCE_OK; }
static inline int sceGxmDestroyContext(SceGxmContext *pContext)
{ (void)pContext; return SCE_OK; }

static inline int sceGxmBeginScene(SceGxmContext *pContext, unsigned int flags,
    SceGxmRenderTarget *pRenderTarget, const SceGxmNotification *pVertexNotif,
    const SceGxmNotification *pFragmentNotif, SceGxmSyncObject *pVertexSyncObject,
    const SceGxmColorSurface *pColorSurface,
    const SceGxmDepthStencilSurface *pDepthStencilSurface)
{
    (void)pContext; (void)flags; (void)pRenderTarget;
    (void)pVertexNotif; (void)pFragmentNotif; (void)pVertexSyncObject;
    (void)pColorSurface; (void)pDepthStencilSurface;
    return SCE_OK;
}

static inline int sceGxmEndScene(SceGxmContext *pContext,
    const SceGxmNotification *pVertexNotif,
    const SceGxmNotification *pFragmentNotif)
{ (void)pContext; (void)pVertexNotif; (void)pFragmentNotif; return SCE_OK; }

static inline void sceGxmFinish(SceGxmContext *pContext) { (void)pContext; }
static inline int sceGxmWaitEvent(void) { return SCE_OK; }
static inline int sceGxmTransferFinish(void) { return SCE_OK; }

static inline int sceGxmSetFrontDepthFunc(SceGxmContext *p, int f)
{ (void)p; (void)f; return SCE_OK; }
static inline int sceGxmSetBackDepthFunc(SceGxmContext *p, int f)
{ (void)p; (void)f; return SCE_OK; }
static inline int sceGxmSetCullMode(SceGxmContext *p, int m)
{ (void)p; (void)m; return SCE_OK; }
static inline int sceGxmSetFrontStencilFunc(SceGxmContext *p,
    int func, int stencilFail, int depthFail, int pass,
    unsigned int compareMask, unsigned int writeMask)
{
    (void)p; (void)func; (void)stencilFail; (void)depthFail;
    (void)pass; (void)compareMask; (void)writeMask;
    return SCE_OK;
}
static inline int sceGxmSetBackStencilFunc(SceGxmContext *p,
    int func, int stencilFail, int depthFail, int pass,
    unsigned int compareMask, unsigned int writeMask)
{
    (void)p; (void)func; (void)stencilFail; (void)depthFail;
    (void)pass; (void)compareMask; (void)writeMask;
    return SCE_OK;
}
static inline int sceGxmSetFrontDepthWriteEnable(SceGxmContext *p, int e)
{ (void)p; (void)e; return SCE_OK; }
static inline int sceGxmSetBackDepthWriteEnable(SceGxmContext *p, int e)
{ (void)p; (void)e; return SCE_OK; }

static inline int sceGxmSetVertexProgram(SceGxmContext *p,
    SceGxmVertexProgram *vp)
{ (void)p; (void)vp; return SCE_OK; }
static inline int sceGxmSetFragmentProgram(SceGxmContext *p,
    SceGxmFragmentProgram *fp)
{ (void)p; (void)fp; return SCE_OK; }

static inline int sceGxmSetVertexStream(SceGxmContext *p, unsigned int idx,
    const void *data)
{ (void)p; (void)idx; (void)data; return SCE_OK; }
static inline int sceGxmSetVertexUniformBuffer(SceGxmContext *p, unsigned int idx,
    const void *data)
{ (void)p; (void)idx; (void)data; return SCE_OK; }
static inline int sceGxmSetFragmentUniformBuffer(SceGxmContext *p, unsigned int idx,
    const void *data)
{ (void)p; (void)idx; (void)data; return SCE_OK; }
static inline int sceGxmSetFragmentTexture(SceGxmContext *p, unsigned int idx,
    const SceGxmTexture *tex)
{ (void)p; (void)idx; (void)tex; return SCE_OK; }

static inline int sceGxmDraw(SceGxmContext *p, int primType, int idxType,
    const void *idxData, unsigned int idxCount)
{
    (void)p; (void)primType; (void)idxType; (void)idxData; (void)idxCount;
    return SCE_OK;
}

static inline int sceGxmReserveVertexDefaultUniformBuffer(SceGxmContext *p,
    void **ppBuf)
{ (void)p; *ppBuf = NULL; return SCE_OK; }
static inline int sceGxmReserveFragmentDefaultUniformBuffer(SceGxmContext *p,
    void **ppBuf)
{ (void)p; *ppBuf = NULL; return SCE_OK; }

/* Texture init */
static inline int sceGxmTextureInitLinear(SceGxmTexture *pTexture, const void *pData,
    int texFormat, unsigned int width, unsigned int height, unsigned int mipCount)
{
    (void)pTexture; (void)pData; (void)texFormat;
    (void)width; (void)height; (void)mipCount;
    return SCE_OK;
}
static inline int sceGxmTextureInitTiled(SceGxmTexture *pTexture, const void *pData,
    int texFormat, unsigned int width, unsigned int height, unsigned int mipCount)
{
    (void)pTexture; (void)pData; (void)texFormat;
    (void)width; (void)height; (void)mipCount;
    return SCE_OK;
}
static inline int sceGxmTextureInitLinearStrided(SceGxmTexture *pTexture,
    const void *pData, int texFormat, unsigned int width, unsigned int height,
    unsigned int byteStride)
{
    (void)pTexture; (void)pData; (void)texFormat;
    (void)width; (void)height; (void)byteStride;
    return SCE_OK;
}
static inline int sceGxmTextureSetData(SceGxmTexture *pTexture, const void *pData)
{ (void)pTexture; (void)pData; return SCE_OK; }
static inline void *sceGxmTextureGetData(const SceGxmTexture *pTexture)
{ (void)pTexture; return NULL; }
static inline unsigned int sceGxmTextureGetWidth(const SceGxmTexture *pTexture)
{ (void)pTexture; return 0; }
static inline unsigned int sceGxmTextureGetHeight(const SceGxmTexture *pTexture)
{ (void)pTexture; return 0; }
static inline int sceGxmTextureGetFormat(const SceGxmTexture *pTexture)
{ (void)pTexture; return SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR; }
static inline int sceGxmTextureSetMinFilter(SceGxmTexture *pTexture, int filter)
{ (void)pTexture; (void)filter; return SCE_OK; }
static inline int sceGxmTextureSetMagFilter(SceGxmTexture *pTexture, int filter)
{ (void)pTexture; (void)filter; return SCE_OK; }
static inline int sceGxmTextureSetMipFilter(SceGxmTexture *pTexture, int filter)
{ (void)pTexture; (void)filter; return SCE_OK; }
static inline int sceGxmTextureSetUAddrMode(SceGxmTexture *pTexture, int mode)
{ (void)pTexture; (void)mode; return SCE_OK; }
static inline int sceGxmTextureSetVAddrMode(SceGxmTexture *pTexture, int mode)
{ (void)pTexture; (void)mode; return SCE_OK; }

/* Shader patcher */
static inline int sceGxmShaderPatcherCreate(const void *params,
    SceGxmShaderPatcher **ppPatcher)
{ (void)params; *ppPatcher = NULL; return SCE_OK; }
static inline int sceGxmShaderPatcherDestroy(SceGxmShaderPatcher *pPatcher)
{ (void)pPatcher; return SCE_OK; }
static inline int sceGxmShaderPatcherRegisterProgram(SceGxmShaderPatcher *p,
    const SceGxmProgram *prog, SceGxmShaderPatcherId *pId)
{
    (void)p; (void)prog;
    if (pId) *pId = 0;
    return SCE_OK;
}
static inline int sceGxmShaderPatcherUnregisterProgram(SceGxmShaderPatcher *p,
    SceGxmShaderPatcherId id)
{ (void)p; (void)id; return SCE_OK; }
static inline int sceGxmShaderPatcherCreateVertexProgram(SceGxmShaderPatcher *p,
    SceGxmShaderPatcherId id, const SceGxmVertexAttribute *attrs,
    unsigned int attrCount, const SceGxmVertexStream *streams,
    unsigned int streamCount, SceGxmVertexProgram **ppVP)
{
    (void)p; (void)id; (void)attrs; (void)attrCount;
    (void)streams; (void)streamCount;
    *ppVP = NULL; return SCE_OK;
}
static inline int sceGxmShaderPatcherCreateFragmentProgram(SceGxmShaderPatcher *p,
    SceGxmShaderPatcherId id, int outputRegFormat, int msMode,
    const SceGxmBlendInfo *pBlend, const SceGxmProgram *matchingVP,
    SceGxmFragmentProgram **ppFP)
{
    (void)p; (void)id; (void)outputRegFormat; (void)msMode;
    (void)pBlend; (void)matchingVP;
    *ppFP = NULL; return SCE_OK;
}
static inline int sceGxmShaderPatcherReleaseVertexProgram(SceGxmShaderPatcher *p,
    SceGxmVertexProgram *vp)
{ (void)p; (void)vp; return SCE_OK; }
static inline int sceGxmShaderPatcherReleaseFragmentProgram(SceGxmShaderPatcher *p,
    SceGxmFragmentProgram *fp)
{ (void)p; (void)fp; return SCE_OK; }

/* Render target */
static inline int sceGxmCreateRenderTarget(const void *params,
    SceGxmRenderTarget **ppTarget)
{ (void)params; *ppTarget = NULL; return SCE_OK; }
static inline int sceGxmDestroyRenderTarget(SceGxmRenderTarget *pTarget)
{ (void)pTarget; return SCE_OK; }

/* Surfaces */
static inline int sceGxmColorSurfaceInit(SceGxmColorSurface *pSurface,
    int colorFormat, int surfType, int scaleMode, int outputRegFormat,
    unsigned int width, unsigned int height, unsigned int strideInPixels,
    void *pData)
{
    (void)pSurface; (void)colorFormat; (void)surfType; (void)scaleMode;
    (void)outputRegFormat; (void)width; (void)height;
    (void)strideInPixels; (void)pData;
    return SCE_OK;
}
static inline int sceGxmDepthStencilSurfaceInit(SceGxmDepthStencilSurface *pSurface,
    int dsFormat, int surfType, unsigned int strideInSamples, void *pDepthData,
    void *pStencilData)
{
    (void)pSurface; (void)dsFormat; (void)surfType;
    (void)strideInSamples; (void)pDepthData; (void)pStencilData;
    return SCE_OK;
}

/* Transfer */
static inline int sceGxmTransferCopy(unsigned int width, unsigned int height,
    unsigned int colorKeyValue, unsigned int colorKeyMask, int colorKeyMode,
    int srcFormat, int srcType, const void *pSrc,
    unsigned int srcX, unsigned int srcY, int srcStride,
    int dstFormat, int dstType, void *pDst,
    unsigned int dstX, unsigned int dstY, int dstStride,
    SceGxmSyncObject *pSyncObject)
{
    (void)width; (void)height; (void)colorKeyValue; (void)colorKeyMask;
    (void)colorKeyMode; (void)srcFormat; (void)srcType; (void)pSrc;
    (void)srcX; (void)srcY; (void)srcStride;
    (void)dstFormat; (void)dstType; (void)pDst;
    (void)dstX; (void)dstY; (void)dstStride; (void)pSyncObject;
    return SCE_OK;
}

/* Notification / sync objects */
static inline int sceGxmSyncObjectCreate(SceGxmSyncObject **ppSyncObject)
{ *ppSyncObject = NULL; return SCE_OK; }
static inline int sceGxmSyncObjectDestroy(SceGxmSyncObject *pSyncObject)
{ (void)pSyncObject; return SCE_OK; }

/* Misc filter/map enum values */
#define SCE_GXM_TEXTURE_FILTER_POINT    0
#define SCE_GXM_TEXTURE_FILTER_LINEAR   1
#define SCE_GXM_TEXTURE_ADDR_CLAMP      1
#define SCE_GXM_TEXTURE_ADDR_REPEAT     2
#define SCE_GXM_TEXTURE_ADDR_MIRROR     3

/* Render surface types */
#define SCE_GXM_COLOR_SURFACE_LINEAR    0
#define SCE_GXM_COLOR_SURFACE_TILED     1
#define SCE_GXM_COLOR_SURFACE_SWIZZLED  2
#define SCE_GXM_DEPTH_STENCIL_SURFACE_TILED 0

/* Primitive types */
#define SCE_GXM_PRIMITIVE_TRIANGLES     0
#define SCE_GXM_PRIMITIVE_TRIANGLE_STRIP 1
#define SCE_GXM_PRIMITIVE_LINES         2

/* Index formats */
#define SCE_GXM_INDEX_FORMAT_U16        0
#define SCE_GXM_INDEX_FORMAT_U32        1

/* Vertex attribute formats */
#define SCE_GXM_ATTRIBUTE_FORMAT_F32    0
#define SCE_GXM_ATTRIBUTE_FORMAT_U8N    1
#define SCE_GXM_ATTRIBUTE_FORMAT_S16N   2

/* Output register format */
#define SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4   0

/* Depth comparison */
#define SCE_GXM_DEPTH_FUNC_LESS_EQUAL   3
#define SCE_GXM_DEPTH_FUNC_ALWAYS       7
#define SCE_GXM_DEPTH_FUNC_LESS         2

/* Cull mode */
#define SCE_GXM_CULL_NONE   0
#define SCE_GXM_CULL_CW     1
#define SCE_GXM_CULL_CCW    2

/* Stencil */
#define SCE_GXM_STENCIL_FUNC_ALWAYS     7
#define SCE_GXM_STENCIL_OP_KEEP         0
#define SCE_GXM_STENCIL_OP_REPLACE      3

/* Depth stencil write enable */
#define SCE_GXM_DEPTH_WRITE_ENABLED     1
#define SCE_GXM_DEPTH_WRITE_DISABLED    0

#define SCE_GXM_NOTIFICATION_COUNT      512

/* GXM program helpers */
static inline const SceGxmProgram *sceGxmShaderPatcherGetProgramFromId(
    SceGxmShaderPatcherId id)
{ (void)id; return NULL; }

#ifdef __cplusplus
}
#endif
