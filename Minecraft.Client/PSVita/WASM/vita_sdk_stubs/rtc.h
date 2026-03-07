#pragma once
/* rtc.h — PS Vita real-time clock API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { uint64_t tick; } SceRtcTick;
typedef struct {
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned int   microsecond;
} SceDateTime;

static inline int sceRtcGetCurrentClock(SceDateTime *pTime, int iTimeZone)
{
    (void)iTimeZone;
    if (!pTime) return -1;
    time_t t = time(NULL);
    struct tm *tm = gmtime(&t);
    pTime->year        = (unsigned short)(tm->tm_year + 1900);
    pTime->month       = (unsigned short)(tm->tm_mon  + 1);
    pTime->day         = (unsigned short)(tm->tm_mday);
    pTime->hour        = (unsigned short)(tm->tm_hour);
    pTime->minute      = (unsigned short)(tm->tm_min);
    pTime->second      = (unsigned short)(tm->tm_sec);
    pTime->microsecond = 0;
    return 0;
}

static inline int sceRtcGetCurrentClockLocalTime(SceDateTime *pTime)
{ return sceRtcGetCurrentClock(pTime, 0); }

static inline unsigned short sceRtcGetYear(const SceDateTime *pTime)
{ return pTime ? pTime->year : 0; }

static inline unsigned short sceRtcGetMonth(const SceDateTime *pTime)
{ return pTime ? pTime->month : 0; }

static inline unsigned short sceRtcGetDay(const SceDateTime *pTime)
{ return pTime ? pTime->day : 0; }

static inline unsigned short sceRtcGetHour(const SceDateTime *pTime)
{ return pTime ? pTime->hour : 0; }

static inline unsigned short sceRtcGetMinute(const SceDateTime *pTime)
{ return pTime ? pTime->minute : 0; }

static inline unsigned short sceRtcGetSecond(const SceDateTime *pTime)
{ return pTime ? pTime->second : 0; }

static inline unsigned int sceRtcGetMicrosecond(const SceDateTime *pTime)
{ return pTime ? pTime->microsecond : 0; }

static inline int sceRtcGetTick(const SceDateTime *pTime, SceRtcTick *pTick)
{
    (void)pTime;
    if (pTick) pTick->tick = 0;
    return 0;
}

static inline int sceRtcSetTick(SceDateTime *pTime, const SceRtcTick *pTick)
{
    (void)pTick;
    if (pTime) {
        pTime->year  = 1970; pTime->month = 1; pTime->day  = 1;
        pTime->hour  = 0;    pTime->minute= 0; pTime->second = 0;
        pTime->microsecond = 0;
    }
    return 0;
}

static inline uint64_t sceRtcGetCurrentTick(SceRtcTick *pTick)
{
    if (pTick) pTick->tick = 0;
    return 0;
}

#ifdef __cplusplus
}
#endif
