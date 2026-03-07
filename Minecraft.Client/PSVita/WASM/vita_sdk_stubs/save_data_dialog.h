#pragma once
#include <stdint.h>
#include <common_dialog.h>

#define SCE_SAVE_DATA_DIALOG_DIRNAME_MAXSIZE 32

typedef struct SceSaveDataDirName { char data[SCE_SAVE_DATA_DIALOG_DIRNAME_MAXSIZE]; } SceSaveDataDirName;

typedef struct SceSaveDataDialogParam {
    uint32_t size;
    int32_t  mode;
    void    *dispType;
} SceSaveDataDialogParam;

typedef struct SceSaveDataDialogResult {
    int32_t  mode;
    int32_t  result;
    int32_t  buttonId;
    uint32_t slotId;
    void    *slotInfo;
    void    *userdata;
} SceSaveDataDialogResult;

typedef struct SceSaveDataDialogFinishParam {
    uint32_t size;
    int32_t  flag;
} SceSaveDataDialogFinishParam;

typedef struct SceSaveDataDialogSystemMessageParam {
    uint32_t size;
    int32_t  sysMsgType;
    int32_t  value;
    void    *userData;
} SceSaveDataDialogSystemMessageParam;

typedef struct SceSaveDataDialogListParam {
    uint32_t size;
    int32_t  listStyle;
} SceSaveDataDialogListParam;

typedef struct SceSaveDataDialogItems {
    void *itemData;
} SceSaveDataDialogItems;
