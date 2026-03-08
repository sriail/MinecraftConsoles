#pragma once

typedef struct SceDateTime {
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned int microsecond;
} SceDateTime;

inline int sceRtcGetCurrentClock(SceDateTime *dateTime, int tz) {
    (void)dateTime; (void)tz;
    return 0;
}

inline int sceRtcGetYear(const SceDateTime *dateTime) {
    return dateTime ? dateTime->year : 0;
}

inline int sceRtcGetMonth(const SceDateTime *dateTime) {
    return dateTime ? dateTime->month : 0;
}

inline int sceRtcGetDay(const SceDateTime *dateTime) {
    return dateTime ? dateTime->day : 0;
}

inline int sceRtcGetDayOfWeek(const SceDateTime *dateTime) {
    (void)dateTime;
    return 0;
}

inline int sceRtcGetHour(const SceDateTime *dateTime) {
    return dateTime ? dateTime->hour : 0;
}

inline int sceRtcGetMinute(const SceDateTime *dateTime) {
    return dateTime ? dateTime->minute : 0;
}

inline int sceRtcGetSecond(const SceDateTime *dateTime) {
    return dateTime ? dateTime->second : 0;
}

inline int sceRtcGetMicrosecond(const SceDateTime *dateTime) {
    return dateTime ? (int)dateTime->microsecond : 0;
}

inline int sceRtcGetTime64_t(const SceDateTime *dateTime, long long *time) {
    (void)dateTime;
    if (time) *time = 0;
    return 0;
}
