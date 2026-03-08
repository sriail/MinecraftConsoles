#pragma once

typedef struct SceTouchReport {
    unsigned char id;
    unsigned char force;
    short x;
    short y;
} SceTouchReport;

typedef struct SceTouchData {
    unsigned long long timeStamp;
    unsigned int status;
    unsigned int reportNum;
    SceTouchReport report[8];
} SceTouchData;
