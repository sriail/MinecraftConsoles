/*
 * shim_SceDisplay.cpp
 * PS Vita SceDisplay → HTML Canvas + requestAnimationFrame shim
 *
 * Build requirements:
 *   -s USE_WEBGL2=1
 *   --js-library shim_SceDisplay.js
 *   The host page must contain: <canvas id="canvas"></canvas>
 */

#include "shim_SceDisplay.h"
#include <string.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#  include <emscripten.h>
#  include <emscripten/html5.h>
#endif

/* Last registered framebuffer descriptor (games poll this) */
static SceDisplayFrameBuf s_current_fb;
static int s_fb_valid = 0;

/* =========================================================================
 * sceDisplaySetFrameBuf
 * VITA:  Registers the current framebuffer with the display hardware
 * WEB:   Blits the ARGB pixel array to the Canvas 2D context via JS glue
 * NOTE:  iUpdateTimingMode == SCE_DISPLAY_SETBUF_NEXTFRAME means the blit
 *        happens on the next vsync; we fire it immediately but do not busy-
 *        wait — the browser compositor handles the actual display timing.
 * ========================================================================= */

int sceDisplaySetFrameBuf(const SceDisplayFrameBuf *pFrameBuf,
                          int iUpdateTimingMode)
{
    (void)iUpdateTimingMode;
    if (!pFrameBuf) return SCE_DISPLAY_ERROR_INVALID_VALUE;
    if (pFrameBuf->width == 0 || pFrameBuf->height == 0)
        return SCE_DISPLAY_ERROR_INVALID_VALUE;

    s_current_fb = *pFrameBuf;
    s_fb_valid   = 1;

#ifdef __EMSCRIPTEN__
    if (pFrameBuf->base) {
        /* Forward to JS glue (shim_SceDisplay.js) which uses
         * ImageData / putImageData on the canvas. */
        EM_ASM({
            vita_display_blit($0, $1, $2, $3);
        }, pFrameBuf->base,
           (int)pFrameBuf->width,
           (int)pFrameBuf->height,
           (int)pFrameBuf->pitch);
    }
#endif
    return SCE_OK;
}

/* =========================================================================
 * sceDisplayGetFrameBuf
 * VITA:  Returns the currently set framebuffer descriptor
 * WEB:   Returns the last descriptor stored by sceDisplaySetFrameBuf
 * ========================================================================= */

int sceDisplayGetFrameBuf(SceDisplayFrameBuf *pFrameBuf,
                          int iUpdateTimingMode)
{
    (void)iUpdateTimingMode;
    if (!pFrameBuf) return SCE_DISPLAY_ERROR_INVALID_VALUE;
    if (!s_fb_valid) {
        memset(pFrameBuf, 0, sizeof(*pFrameBuf));
        return SCE_DISPLAY_ERROR_INVALID_VALUE;
    }
    *pFrameBuf = s_current_fb;
    return SCE_OK;
}

/* =========================================================================
 * sceDisplayWaitVblankStart
 * VITA:  Blocks the calling thread until the next vertical blank interrupt
 * WEB:   Yields control to the browser event loop for one animation frame
 *        (~16.67 ms at 60 Hz) using Asyncify sleep.
 * NOTE:  Requires -s ASYNCIFY=1 and -s ASYNCIFY_IMPORTS=["emscripten_sleep"]
 * ========================================================================= */

int sceDisplayWaitVblankStart(void)
{
#ifdef __EMSCRIPTEN__
    emscripten_sleep(17); /* ~60 Hz */
#endif
    return SCE_OK;
}

int sceDisplayWaitVblankStartMulti(unsigned int vcount)
{
#ifdef __EMSCRIPTEN__
    emscripten_sleep((unsigned int)(17u * vcount));
#endif
    return SCE_OK;
}

/* =========================================================================
 * sceDisplayGetRefreshRate
 * VITA:  Returns display refresh rate in Hz (always 60.0 on Vita)
 * WEB:   Returns 60.0f (Canvas does not expose a refresh rate query)
 * ========================================================================= */

int sceDisplayGetRefreshRate(float *pFps)
{
    if (!pFps) return SCE_DISPLAY_ERROR_INVALID_VALUE;
    *pFps = 60.0f;
    return SCE_OK;
}

/* =========================================================================
 * sceDisplayGetMaximumFrameBufResolution
 * VITA:  Returns 960 × 544 (native Vita display)
 * WEB:   Returns the Canvas element dimensions clamped to 960 × 544
 * ========================================================================= */

int sceDisplayGetMaximumFrameBufResolution(int *pWidth, int *pHeight)
{
    if (!pWidth || !pHeight) return SCE_DISPLAY_ERROR_INVALID_VALUE;
    *pWidth  = 960;
    *pHeight = 544;
#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size("#canvas", pWidth, pHeight);
#endif
    return SCE_OK;
}
