#pragma once

typedef struct SceSaveDataDialogParam {
    unsigned char data[256];
} SceSaveDataDialogParam;

typedef struct SceSaveDataDialogResult {
    int mode;
    int result;
    unsigned char reserved[32];
} SceSaveDataDialogResult;

#define SCE_SAVEDATA_DIALOG_STATUS_NONE     0
#define SCE_SAVEDATA_DIALOG_STATUS_RUNNING  1
#define SCE_SAVEDATA_DIALOG_STATUS_FINISHED 2

inline int sceSaveDataDialogInit(const SceSaveDataDialogParam *param) {
    (void)param;
    return 0;
}

inline int sceSaveDataDialogGetResult(SceSaveDataDialogResult *result) {
    (void)result;
    return 0;
}

inline int sceSaveDataDialogTerm(void) {
    return 0;
}

inline int sceSaveDataDialogGetStatus(void) {
    return SCE_SAVEDATA_DIALOG_STATUS_NONE;
}
