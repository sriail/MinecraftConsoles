#pragma once
#include <stdint.h>

typedef int SceCommonDialogStatus;
#define SCE_COMMON_DIALOG_STATUS_NONE     0
#define SCE_COMMON_DIALOG_STATUS_RUNNING  1
#define SCE_COMMON_DIALOG_STATUS_FINISHED 2

typedef struct SceCommonDialogColor {
    uint8_t r, g, b, a;
} SceCommonDialogColor;
