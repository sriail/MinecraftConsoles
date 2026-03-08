#pragma once

typedef struct SceNpDrmLicense {
    unsigned char data[512];
} SceNpDrmLicense;

inline int sceNpDrmIsAvailable(const SceNpDrmLicense *license, const char *skuPath) {
    (void)license; (void)skuPath;
    return 0;
}

inline int sceNpDrmGetContentId(const char *path, char *contentId, unsigned int size) {
    (void)path; (void)contentId; (void)size;
    return 0;
}
