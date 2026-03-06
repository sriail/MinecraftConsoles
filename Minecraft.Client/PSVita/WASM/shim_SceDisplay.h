#ifndef SHIM_SCEDISPLAY_H
#define SHIM_SCEDISPLAY_H

/*
 * shim_SceDisplay.h
 * PS Vita SceDisplay → HTML Canvas + requestAnimationFrame shim
 *
 * Covers: framebuffer registration, vsync, display mode query.
 * Native Vita resolution: 960 × 544 @ 60 Hz.
 *
 * ABI reference: vitasdk <display.h>
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
#define SCE_DISPLAY_ERROR_INVALID_HEAD  ((int)0x80290001)
#define SCE_DISPLAY_ERROR_INVALID_VALUE ((int)0x80290002)

/* ---------- Constants ---------- */
#define SCE_DISPLAY_PIXELFORMAT_A8B8G8R8   0x00000000
#define SCE_DISPLAY_SETBUF_NEXTFRAME       0
#define SCE_DISPLAY_SETBUF_IMMEDIATE       1

/* ---------- Display head ---------- */
#define SCE_DISPLAY_HEAD_MAIN   0
#define SCE_DISPLAY_HEAD_OLED   1  /* Vita OLED model */

/* ---------- Structs ---------- */
typedef struct SceDisplayFrameBuf {
    uint32_t size;         /* sizeof this struct */
    void    *base;         /* pointer to ARGB pixel data in linear memory */
    uint32_t pitch;        /* pixels per row (≥ width) */
    uint32_t pixelformat;
    uint32_t width;
    uint32_t height;
} SceDisplayFrameBuf;

/* ---------- API ---------- */
int sceDisplaySetFrameBuf(const SceDisplayFrameBuf *pFrameBuf, int iUpdateTimingMode);
int sceDisplayGetFrameBuf(SceDisplayFrameBuf *pFrameBuf, int iUpdateTimingMode);
int sceDisplayWaitVblankStart(void);
int sceDisplayWaitVblankStartMulti(unsigned int vcount);
int sceDisplayGetRefreshRate(float *pFps);
int sceDisplayGetMaximumFrameBufResolution(int *pWidth, int *pHeight);

#ifdef __cplusplus
}
#endif

#endif /* SHIM_SCEDISPLAY_H */
