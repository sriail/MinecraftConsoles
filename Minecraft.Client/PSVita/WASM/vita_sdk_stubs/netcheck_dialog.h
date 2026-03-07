#pragma once
#include <stdint.h>

typedef struct SceNetCheckDialogParam {
    uint32_t size;
} SceNetCheckDialogParam;

typedef struct SceNetCheckDialogResult {
    int32_t result;
} SceNetCheckDialogResult;

typedef struct SceNetCheckDialogAgeRestriction {
    int32_t countryCode;
    int32_t age;
} SceNetCheckDialogAgeRestriction;
