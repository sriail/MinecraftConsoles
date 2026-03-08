#pragma once

typedef struct SceMsgDialogParam {
    unsigned char data[256];
} SceMsgDialogParam;

typedef struct SceMsgDialogResult {
    int mode;
    int result;
    int buttonId;
    unsigned char reserved[32];
} SceMsgDialogResult;

#define SCE_MSG_DIALOG_BUTTON_TYPE_OK           0
#define SCE_MSG_DIALOG_BUTTON_TYPE_YESNO        1
#define SCE_MSG_DIALOG_BUTTON_TYPE_NONE         2
#define SCE_MSG_DIALOG_BUTTON_TYPE_OK_CANCEL    3

#define SCE_MSG_DIALOG_BUTTON_ID_OK       1
#define SCE_MSG_DIALOG_BUTTON_ID_YES      1
#define SCE_MSG_DIALOG_BUTTON_ID_NO       2
#define SCE_MSG_DIALOG_BUTTON_ID_CANCEL   3
#define SCE_MSG_DIALOG_BUTTON_ID_INVALID  0

#define SCE_COMMON_DIALOG_STATUS_NONE      0
#define SCE_COMMON_DIALOG_STATUS_RUNNING   1
#define SCE_COMMON_DIALOG_STATUS_FINISHED  2

typedef int SceMsgDialogMode;
#define SCE_MSG_DIALOG_MODE_USER_MSG  1
#define SCE_MSG_DIALOG_MODE_SYSTEM_MSG 2

inline int sceMsgDialogInit(const SceMsgDialogParam *param) {
    (void)param;
    return 0;
}

inline int sceMsgDialogGetResult(SceMsgDialogResult *result) {
    (void)result;
    return 0;
}

inline int sceMsgDialogTerm(void) {
    return 0;
}

inline int sceMsgDialogGetStatus(void) {
    return SCE_COMMON_DIALOG_STATUS_NONE;
}
