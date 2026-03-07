#pragma once
/* np/np_basic.h — PS Vita NP Basic API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <string.h>
#include "np_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Basic event types */
#define SCE_NP_BASIC_EVENT_INCOMING_BOOTABLE_INVITATION  0x1001
#define SCE_NP_BASIC_EVENT_RECV_INVITATION_RESULT        0x1002

/* Friend context state */
#define SCE_NP_BASIC_FRIEND_CONTEXT_STATE_UNKNOWN        0
#define SCE_NP_BASIC_FRIEND_CONTEXT_STATE_IN_CONTEXT     1

typedef int SceNpBasicFriendContextState;

/* Presence */
#define SCE_NP_BASIC_IN_GAME_PRESENCE_TYPE_DEFAULT       0
#define SCE_NP_BASIC_IN_GAME_PRESENCE_TYPE_GAME_JOINING  1
#define SCE_NP_BASIC_IN_GAME_PRESENCE_DATA_SIZE_MAX      128

typedef struct {
    int      presenceType;
    uint8_t  data[SCE_NP_BASIC_IN_GAME_PRESENCE_DATA_SIZE_MAX];
    uint32_t dataSize;
} SceNpBasicGamePresence;

/* Attachment / message */
#define SCE_NP_BASIC_INVALID_ATTACHMENT_DATA_ID  0
#define SCE_NP_BASIC_MAX_MESSAGE_SIZE            1024
#define SCE_NP_BASIC_MESSAGE_MAIN_TYPE_INVITE    1
#define SCE_NP_BASIC_MESSAGE_ACTION_ACCEPT       1
#define SCE_NP_BASIC_RECV_MESSAGE_OPTIONS_INCLUDE_BOOTABLE  0x01
#define SCE_NP_BASIC_SELECTED_INVITATION_DATA    0x01

typedef uint32_t SceNpBasicAttachmentDataId;

typedef struct {
    uint8_t data[256];
} SceNpBasicExtendedAttachmentData;

/* Event handler */
typedef void (*SceNpBasicEventHandler)(int event, int retCode,
    unsigned int reqId, void *arg);

/* Error codes */
#define SCE_NP_BASIC_ERROR_BUSY  -2137456639

/* API stubs */
static inline int sceNpBasicInit(unsigned int poolSize)
{ (void)poolSize; return SCE_OK; }

static inline int sceNpBasicTerm(void) { return SCE_OK; }

static inline int sceNpBasicRegisterHandler(SceNpBasicEventHandler handler, void *arg)
{ (void)handler; (void)arg; return SCE_OK; }

static inline int sceNpBasicGetFriendListEntryCount(unsigned int *pCount)
{ if (pCount) *pCount = 0; return SCE_OK; }

static inline int sceNpBasicGetFriendListEntries(unsigned int startIdx,
    SceNpId *pList, unsigned int listMax, unsigned int *pCount)
{
    (void)startIdx; (void)pList; (void)listMax;
    if (pCount) *pCount = 0;
    return SCE_OK;
}

static inline int sceNpBasicGetFriendContextState(const SceNpId *npId,
    SceNpBasicFriendContextState *pState)
{
    (void)npId;
    if (pState) *pState = SCE_NP_BASIC_FRIEND_CONTEXT_STATE_UNKNOWN;
    return SCE_OK;
}

static inline int sceNpBasicGetGamePresenceOfFriend(const SceNpId *npId,
    SceNpBasicGamePresence *pPresence)
{
    (void)npId;
    if (pPresence) { memset(pPresence, 0, sizeof(*pPresence)); }
    return SCE_OK;
}

static inline int sceNpBasicSendMessageAttachment(const SceNpId *npId,
    const char *subject, const char *body,
    const void *data, size_t dataSize, unsigned int *pReqId)
{
    (void)npId; (void)subject; (void)body; (void)data; (void)dataSize;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpBasicGetMessageAttachmentEntryCount(uint8_t mainType,
    unsigned int options, unsigned int *pCount)
{
    (void)mainType; (void)options;
    if (pCount) *pCount = 0;
    return SCE_OK;
}

static inline int sceNpBasicGetMessageAttachmentEntry(unsigned int idx,
    SceNpBasicAttachmentDataId *pId)
{
    (void)idx;
    if (pId) *pId = SCE_NP_BASIC_INVALID_ATTACHMENT_DATA_ID;
    return -1;
}

static inline int sceNpBasicGetExtendedAttachmentData(SceNpBasicAttachmentDataId id,
    SceNpBasicExtendedAttachmentData *pData, unsigned int *pReqId)
{
    (void)id; (void)pData;
    if (pReqId) *pReqId = 0;
    return -1;
}

#ifdef __cplusplus
}
#endif
