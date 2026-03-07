#pragma once
/* touch.h — PS Vita touch input stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_TOUCH_PORT_FRONT     0
#define SCE_TOUCH_PORT_BACK      1
#define SCE_TOUCH_PORT_MAX_NUM   2

#define SCE_TOUCH_SAMPLING_STATE_STOP    0
#define SCE_TOUCH_SAMPLING_STATE_START   1

#define SCE_TOUCH_MAX_REPORT             8

typedef struct {
    int16_t x;
    int16_t y;
    uint16_t id;
    uint8_t  pad[2];
    uint32_t pad2[5];
} SceTouchReport;

typedef struct {
    uint32_t       timeStamp;
    uint8_t        status;
    uint8_t        reportNum;
    uint8_t        pad[2];
    SceTouchReport report[SCE_TOUCH_MAX_REPORT];
} SceTouchData;

static inline int sceTouchSetSamplingState(int port, int state)
{ (void)port; (void)state; return SCE_OK; }

static inline int sceTouchRead(int port, SceTouchData *pData, int nBufs)
{
    (void)port; (void)nBufs;
    if (pData) { pData->reportNum = 0; pData->status = 0; }
    return 1;
}

static inline int sceTouchPeek(int port, SceTouchData *pData, int nBufs)
{ return sceTouchRead(port, pData, nBufs); }

static inline int sceTouchEnableTouchForce(int port)
{ (void)port; return SCE_OK; }

static inline int sceTouchDisableTouchForce(int port)
{ (void)port; return SCE_OK; }

#ifdef __cplusplus
}
#endif
