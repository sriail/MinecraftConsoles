#pragma once
#include <stdint.h>

#define SCE_TOUCH_PORT_FRONT 0
#define SCE_TOUCH_PORT_BACK  1
#define SCE_TOUCH_MAX_REPORT 8

typedef struct SceTouchReport {
    uint8_t id;
    uint8_t force;
    int16_t x;
    int16_t y;
    int8_t  reserved[8];
    uint16_t info;
} SceTouchReport;

typedef struct SceTouchData {
    uint64_t timeStamp;
    uint32_t status;
    uint32_t reportNum;
    SceTouchReport report[SCE_TOUCH_MAX_REPORT];
} SceTouchData;

typedef struct SceTouchPanelInfo {
    int16_t minAaX, minAaY, maxAaX, maxAaY;
    int16_t minDispX, minDispY, maxDispX, maxDispY;
    uint8_t minForce, maxForce;
    uint8_t reserved[30];
} SceTouchPanelInfo;
