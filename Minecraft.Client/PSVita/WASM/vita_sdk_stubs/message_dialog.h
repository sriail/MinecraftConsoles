#pragma once
/*
 * Minimal PS Vita message dialog SDK stubs for Emscripten/WASM builds.
 */

#include <stdint.h>

typedef enum SceMsgDialogSystemMessageType {
    SCE_MSG_DIALOG_SYSMSG_TYPE_INVALID                      = 0,
    SCE_MSG_DIALOG_SYSMSG_TYPE_WAIT                         = 1,
    SCE_MSG_DIALOG_SYSMSG_TYPE_NOSPACE                      = 2,
    SCE_MSG_DIALOG_SYSMSG_TYPE_WAIT_SMALL                   = 3,
    SCE_MSG_DIALOG_SYSMSG_TYPE_NEED_CONTINUE_SENDING_EVENT  = 4,
    SCE_MSG_DIALOG_SYSMSG_TYPE_APPROACHING_TIME_LIMIT       = 5,
    SCE_MSG_DIALOG_SYSMSG_TYPE_NOSPACE_CONTINUABLE          = 6,
} SceMsgDialogSystemMessageType;
