#pragma once

typedef struct SceCtrlData {
    unsigned long long timeStamp;
    unsigned int buttons;
    unsigned char lx;
    unsigned char ly;
    unsigned char rx;
    unsigned char ry;
    unsigned char reserved[4];
} SceCtrlData;
