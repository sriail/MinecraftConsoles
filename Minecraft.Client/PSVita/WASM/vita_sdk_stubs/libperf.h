#pragma once

inline int scePerfArmPmonReset(int threadId) {
    (void)threadId;
    return 0;
}

inline int scePerfArmPmonSelectEvent(int threadId, int counter, int event) {
    (void)threadId; (void)counter; (void)event;
    return 0;
}

inline int scePerfArmPmonStart(int threadId) {
    (void)threadId;
    return 0;
}

inline int scePerfArmPmonStop(int threadId) {
    (void)threadId;
    return 0;
}

inline int scePerfArmPmonGetCounterValue(int threadId, int counter, unsigned int *value) {
    (void)threadId; (void)counter;
    if (value) *value = 0;
    return 0;
}
