#pragma once

typedef int SceNpBasicEventHandler;

typedef struct SceNpBasicAttachmentData {
    void *data;
    unsigned int size;
} SceNpBasicAttachmentData;

inline int sceNpBasicInit(void *param) {
    (void)param;
    return 0;
}

inline int sceNpBasicTerm(void) {
    return 0;
}

inline int sceNpBasicRegisterHandler(void *handler, void *arg, int *id) {
    (void)handler; (void)arg;
    if (id) *id = 0;
    return 0;
}

inline int sceNpBasicUnregisterHandler(int id) {
    (void)id;
    return 0;
}
