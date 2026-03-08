#pragma once

typedef struct SceGxmTexture {
    unsigned int data[4];
} SceGxmTexture;

typedef struct SceGxmContext SceGxmContext;

typedef int SceGxmStencilFunc;

typedef struct SceGxmShaderPatcher SceGxmShaderPatcher;

typedef int SceGxmShaderPatcherId;

typedef struct SceGxmVertexProgram SceGxmVertexProgram;
typedef struct SceGxmFragmentProgram SceGxmFragmentProgram;
typedef struct SceGxmProgram SceGxmProgram;
typedef struct SceGxmRenderTarget SceGxmRenderTarget;
typedef struct SceGxmColorSurface SceGxmColorSurface;
typedef struct SceGxmDepthStencilSurface SceGxmDepthStencilSurface;

#define SCE_GXM_STENCIL_FUNC_NEVER    0
#define SCE_GXM_STENCIL_FUNC_LESS     1
#define SCE_GXM_STENCIL_FUNC_EQUAL    2
#define SCE_GXM_STENCIL_FUNC_ALWAYS   7
