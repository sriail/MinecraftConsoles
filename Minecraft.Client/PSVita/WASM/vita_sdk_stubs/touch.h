#pragma once
// Minimal PS Vita touch.h stub for Emscripten/WASM builds.

#include <stdint.h>

#define SCE_TOUCH_MAX_REPORT 8

typedef struct SceTouchReport {
    uint8_t  id;
    uint8_t  flags;
    uint16_t x;
    uint16_t y;
    uint16_t force;
    uint16_t _reserved;
} SceTouchReport;

typedef struct SceTouchData {
    uint64_t    timeStamp;
    uint8_t     status;
    uint8_t     reportNum;
    uint8_t     _reserved[6];
    SceTouchReport report[SCE_TOUCH_MAX_REPORT];
} SceTouchData;

#define SCE_TOUCH_PORT_FRONT 0
#define SCE_TOUCH_PORT_BACK  1

#ifdef __cplusplus
extern "C" {
#endif

int sceTouchGetData(int port, SceTouchData *pData, int nBufs);
int sceTouchSetSamplingState(int port, int state);
int sceTouchEnableTouchForce(int port);
int sceTouchDisableTouchForce(int port);

#ifdef __cplusplus
}
#endif
