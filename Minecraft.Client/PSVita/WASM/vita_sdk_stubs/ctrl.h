#pragma once
/* ctrl.h — PS Vita controller input stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Button bitmask values */
#define SCE_CTRL_SELECT       (1u << 0)
#define SCE_CTRL_L3           (1u << 1)
#define SCE_CTRL_R3           (1u << 2)
#define SCE_CTRL_START        (1u << 3)
#define SCE_CTRL_UP           (1u << 4)
#define SCE_CTRL_RIGHT        (1u << 5)
#define SCE_CTRL_DOWN         (1u << 6)
#define SCE_CTRL_LEFT         (1u << 7)
#define SCE_CTRL_LTRIGGER     (1u << 8)
#define SCE_CTRL_RTRIGGER     (1u << 9)
#define SCE_CTRL_L1           SCE_CTRL_LTRIGGER
#define SCE_CTRL_R1           SCE_CTRL_RTRIGGER
#define SCE_CTRL_TRIANGLE     (1u << 12)
#define SCE_CTRL_CIRCLE       (1u << 13)
#define SCE_CTRL_CROSS        (1u << 14)
#define SCE_CTRL_SQUARE       (1u << 15)
#define SCE_CTRL_INTERCEPTED  (1u << 16)
#define SCE_CTRL_VOLUP        (1u << 17)
#define SCE_CTRL_VOLDOWN      (1u << 18)
#define SCE_CTRL_PSBUTTON     (1u << 27)
#define SCE_CTRL_HEADPHONE    (1u << 28)

/* Sampling modes */
#define SCE_CTRL_MODE_DIGITAL        0
#define SCE_CTRL_MODE_ANALOG         1
#define SCE_CTRL_MODE_ANALOG_WIDE    2
#define SCE_CTRL_ANALOG_MODE_DIGITALANALOG  SCE_CTRL_MODE_ANALOG

typedef struct {
    unsigned int  timeStamp;
    unsigned int  buttons;
    unsigned char lx;
    unsigned char ly;
    unsigned char rx;
    unsigned char ry;
    unsigned char lx2;
    unsigned char ly2;
    unsigned char rx2;
    unsigned char ry2;
    unsigned char reserved[12];
} SceCtrlData;

typedef SceCtrlData SceCtrlData2;

static inline int sceCtrlSetSamplingMode(unsigned int mode)
{ (void)mode; return SCE_OK; }

static inline int sceCtrlSetSamplingModeExt(unsigned int mode)
{ (void)mode; return SCE_OK; }

static inline int sceCtrlReadBufferPositive(int port, SceCtrlData *pad, int count)
{
    (void)port; (void)count;
    if (pad) { pad->buttons = 0; pad->lx = 128; pad->ly = 128; pad->rx = 128; pad->ry = 128; }
    return 1;
}

static inline int sceCtrlReadBufferPositive2(int port, SceCtrlData2 *pad, int count)
{
    (void)port; (void)count;
    if (pad) { pad->buttons = 0; pad->lx = 128; pad->ly = 128; pad->rx = 128; pad->ry = 128; }
    return 1;
}

static inline int sceCtrlReadBufferNegative(int port, SceCtrlData *pad, int count)
{
    (void)port; (void)count;
    if (pad) { pad->buttons = 0; pad->lx = 128; pad->ly = 128; pad->rx = 128; pad->ry = 128; }
    return 1;
}

static inline int sceCtrlPeekBufferPositive(int port, SceCtrlData *pad, int count)
{ return sceCtrlReadBufferPositive(port, pad, count); }

#ifdef __cplusplus
}
#endif
