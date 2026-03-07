#pragma once
// Minimal PS Vita message_dialog.h stub for Emscripten/WASM builds.
// Covers the SaveData dialog and CommonDialog types used by this codebase.

#include <stdint.h>

// Common dialog status
typedef enum SceCommonDialogStatus {
    SCE_COMMON_DIALOG_STATUS_NONE       = 0,
    SCE_COMMON_DIALOG_STATUS_RUNNING    = 1,
    SCE_COMMON_DIALOG_STATUS_FINISHED   = 2
} SceCommonDialogStatus;

// Common dialog result
typedef enum SceCommonDialogResult {
    SCE_COMMON_DIALOG_RESULT_OK        = 0,
    SCE_COMMON_DIALOG_RESULT_USER_CANCELED = 1,
    SCE_COMMON_DIALOG_RESULT_ABORT     = 2
} SceCommonDialogResult;

// Save data dialog system message type
typedef enum SceSaveDataDialogSystemMessageType {
    SCE_SAVEDATA_DIALOG_SYSMSG_TYPE_NODATA          = 1,
    SCE_SAVEDATA_DIALOG_SYSMSG_TYPE_NOSPACE         = 2,
    SCE_SAVEDATA_DIALOG_SYSMSG_TYPE_NOSPACE_CONTINUABLE = 3
} SceSaveDataDialogSystemMessageType;

typedef struct SceSaveDataDialogSystemMessageParam {
    SceSaveDataDialogSystemMessageType sysMsgType;
    uint32_t value;
    uint8_t  reserved[32];
} SceSaveDataDialogSystemMessageParam;

typedef struct SceSaveDataDialogListParam {
    uint32_t *focusSaveDataDirName;
    uint8_t  reserved[32];
} SceSaveDataDialogListParam;

typedef struct SceSaveDataDialogParam {
    uint32_t version;
    uint32_t mode;
    uint32_t dispType;
    uint32_t _reserved;
    SceSaveDataDialogSystemMessageParam *sysMsgParam;
    SceSaveDataDialogListParam          *listParam;
    void     *newSaveDataInfo;
    void     *userdata;
    uint8_t   reserved[32];
} SceSaveDataDialogParam;

typedef struct SceSaveDataDialogFinishParam {
    uint32_t mode;
    uint8_t  reserved[32];
} SceSaveDataDialogFinishParam;

typedef struct SceSaveDataDialogResult {
    uint32_t mode;
    SceCommonDialogResult result;
    uint32_t buttonId;
    wchar_t *dirName;
    void    *param;
    uint8_t  reserved[16];
} SceSaveDataDialogResult;

#define SCE_SAVEDATA_DIALOG_MODE_FIXED    1
#define SCE_SAVEDATA_DIALOG_MODE_LIST     2
#define SCE_SAVEDATA_DIALOG_MODE_USER_MSG 3
#define SCE_SAVEDATA_DIALOG_MODE_SYSTEM_MSG 4

#define SCE_SAVEDATA_DIALOG_DISP_TYPE_DEFAULT 0

#ifdef __cplusplus
extern "C" {
#endif

int sceSaveDataDialogInit(void *initParam);
int sceSaveDataDialogTerm(void);
void sceSaveDataDialogParamInit(SceSaveDataDialogParam *param);
int sceSaveDataDialogOpen(const SceSaveDataDialogParam *param);
int sceSaveDataDialogClose(void);
int sceSaveDataDialogContinue(const SceSaveDataDialogParam *param);
SceCommonDialogStatus sceSaveDataDialogGetStatus(void);
SceCommonDialogStatus sceSaveDataDialogGetSubStatus(void);
int sceSaveDataDialogGetResult(SceSaveDataDialogResult *result);
int sceSaveDataDialogFinish(const SceSaveDataDialogFinishParam *param);
int sceSaveDataDialogProgressBarInc(uint32_t target, uint32_t delta);

#ifdef __cplusplus
}
#endif
