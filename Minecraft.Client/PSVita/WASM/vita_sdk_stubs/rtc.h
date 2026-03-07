#pragma once
// Minimal PS Vita rtc.h stub for Emscripten/WASM builds.

#include <stdint.h>

typedef struct SceDateTime {
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;
    uint32_t microsecond;
} SceDateTime;

typedef uint64_t SceUInt64;

#ifdef __cplusplus
extern "C" {
#endif

int      sceRtcGetCurrentClock(SceDateTime *pTime, int timeZone);
int      sceRtcGetCurrentClockLocalTime(SceDateTime *pTime);
int      sceRtcGetTime64_t(const SceDateTime *pTime, uint64_t *pTime64);
uint16_t sceRtcGetYear(const SceDateTime *pTime);
uint16_t sceRtcGetMonth(const SceDateTime *pTime);
uint16_t sceRtcGetDay(const SceDateTime *pTime);
uint16_t sceRtcGetDayOfWeek(int year, int month, int day);
uint16_t sceRtcGetHour(const SceDateTime *pTime);
uint16_t sceRtcGetMinute(const SceDateTime *pTime);
uint16_t sceRtcGetSecond(const SceDateTime *pTime);
uint32_t sceRtcGetMicrosecond(const SceDateTime *pTime);

#ifdef __cplusplus
}
#endif
