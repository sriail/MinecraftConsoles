#pragma once
#include <stdint.h>

#define SCE_CTRL_SELECT   0x00000001
#define SCE_CTRL_L3       0x00000002
#define SCE_CTRL_R3       0x00000004
#define SCE_CTRL_START    0x00000008
#define SCE_CTRL_UP       0x00000010
#define SCE_CTRL_RIGHT    0x00000020
#define SCE_CTRL_DOWN     0x00000040
#define SCE_CTRL_LEFT     0x00000080
#define SCE_CTRL_LTRIGGER 0x00000100
#define SCE_CTRL_RTRIGGER 0x00000200
#define SCE_CTRL_L1       0x00000400
#define SCE_CTRL_R1       0x00000800
#define SCE_CTRL_TRIANGLE 0x00001000
#define SCE_CTRL_CIRCLE   0x00002000
#define SCE_CTRL_CROSS    0x00004000
#define SCE_CTRL_SQUARE   0x00008000

typedef struct SceCtrlData {
    uint64_t timeStamp;
    uint32_t buttons;
    uint8_t  lx;
    uint8_t  ly;
    uint8_t  rx;
    uint8_t  ry;
    uint8_t  reserved[16];
} SceCtrlData;

typedef struct SceCtrlData2 {
    uint64_t timeStamp;
    uint32_t buttons;
    uint8_t  lx;
    uint8_t  ly;
    uint8_t  rx;
    uint8_t  ry;
    uint8_t  reserved[16];
} SceCtrlData2;
