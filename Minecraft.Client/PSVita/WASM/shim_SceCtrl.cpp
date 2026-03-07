/*
 * shim_SceCtrl.cpp
 * PS Vita SceCtrl → Web Gamepad API shim implementation
 *
 * Build requirements:
 *   --js-library shim_SceCtrl.js
 *
 * Thread safety:
 *   sceCtrlReadBufferPositive is called from the game's main/render thread.
 *   The JS side updates a SharedArrayBuffer (when --shared-memory is enabled)
 *   or a plain JS object polled via vita_ctrl_read().
 *   Single-threaded WASM builds poll the Gamepad API directly in the JS glue.
 */

#include "shim_SceCtrl.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#  include <emscripten.h>
#endif

/* =========================================================================
 * Internal state populated by the JS glue layer
 * ========================================================================= */

/* Raw gamepad state struct shared between C and JS */
typedef struct {
    uint32_t buttons;
    float    axes[4]; /* lx, ly, rx, ry  in range -1..1 */
    float    lt;      /* L2 0..1 */
    float    rt;      /* R2 0..1 */
} RawGamepadState;

static RawGamepadState s_gp_state;
static int s_sampling_mode = SCE_CTRL_MODE_DIGITAL;

/* =========================================================================
 * sceCtrlSetSamplingMode
 * VITA:  Configures whether analog data is returned in SceCtrlData
 * WEB:   Stores the mode; all reads always return full analog data anyway
 * ========================================================================= */

int sceCtrlSetSamplingMode(int mode)
{
    s_sampling_mode = mode;
    return SCE_OK;
}

/* =========================================================================
 * vita_ctrl_read (called from shim_SceCtrl.js via JS→C)
 * Stores the latest gamepad state into s_gp_state.
 * ========================================================================= */

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void vita_ctrl_update(uint32_t buttons,
                      float lx, float ly, float rx, float ry,
                      float lt, float rt)
{
    s_gp_state.buttons  = buttons;
    s_gp_state.axes[0]  = lx;
    s_gp_state.axes[1]  = ly;
    s_gp_state.axes[2]  = rx;
    s_gp_state.axes[3]  = ry;
    s_gp_state.lt       = lt;
    s_gp_state.rt       = rt;
}

/* =========================================================================
 * sceCtrlReadBufferPositive
 * VITA:  Reads the latest controller state; blocks until a new sample
 * WEB:   Polls the JS Gamepad API snapshot (updated each rAF tick)
 * NOTE:  port 0 is always the first connected gamepad; port > 0 → INVALID_ARG
 * ========================================================================= */

static void poll_gamepad(void)
{
#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_ctrl_poll(); });
#endif
}

static void fill_sce_ctrl_data(SceCtrlData *pData)
{
    memset(pData, 0, sizeof(*pData));

    /* Timestamp: microseconds */
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    pData->timeStamp = (uint64_t)ts.tv_sec * 1000000ULL +
                       (uint64_t)(ts.tv_nsec / 1000ULL);

    pData->buttons = s_gp_state.buttons;

    /* Map -1..1 float axes → 0..255 byte (128 = centre) */
    pData->lx = (uint8_t)((s_gp_state.axes[0] * 127.0f) + 128.0f);
    pData->ly = (uint8_t)((s_gp_state.axes[1] * 127.0f) + 128.0f);
    pData->rx = (uint8_t)((s_gp_state.axes[2] * 127.0f) + 128.0f);
    pData->ry = (uint8_t)((s_gp_state.axes[3] * 127.0f) + 128.0f);

    pData->lt = (uint8_t)(s_gp_state.lt * 255.0f);
    pData->rt = (uint8_t)(s_gp_state.rt * 255.0f);
}

int sceCtrlReadBufferPositive(int port, SceCtrlData *pData, int count)
{
    if (!pData || count <= 0) return SCE_CTRL_ERROR_INVALID_ARG;
    if (port != 0) return SCE_CTRL_ERROR_INVALID_ARG;

    poll_gamepad();

    for (int i = 0; i < count; i++) fill_sce_ctrl_data(&pData[i]);
    return count;
}

int sceCtrlReadBufferNegative(int port, SceCtrlData *pData, int count)
{
    /* Negative buffer returns buttons that are NOT pressed */
    if (!pData || count <= 0) return SCE_CTRL_ERROR_INVALID_ARG;
    if (port != 0) return SCE_CTRL_ERROR_INVALID_ARG;

    poll_gamepad();

    for (int i = 0; i < count; i++) {
        fill_sce_ctrl_data(&pData[i]);
        pData[i].buttons = ~pData[i].buttons;
    }
    return count;
}

int sceCtrlPeekBufferPositive(int port, SceCtrlData *pData, int count)
{
    return sceCtrlReadBufferPositive(port, pData, count);
}
