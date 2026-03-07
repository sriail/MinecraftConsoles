#pragma once
#include <stdint.h>
#include <np.h>
#include <common_dialog.h>

typedef struct SceGameCustomDataDialogDataParam { uint32_t size; } SceGameCustomDataDialogDataParam;
typedef struct SceGameCustomDataDialogParam { uint32_t size; } SceGameCustomDataDialogParam;
typedef struct SceGameCustomDataDialogResult { int32_t result; } SceGameCustomDataDialogResult;
typedef struct SceGameCustomDataDialogOnlineIdList { int32_t count; SceNpOnlineId *list; } SceGameCustomDataDialogOnlineIdList;
