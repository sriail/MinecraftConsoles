#pragma once
// Minimal PS Vita np.h stub for Emscripten/WASM builds.
// Provides all NP/PSN types referenced by this codebase.

#include <stdint.h>
#include <string.h>

// -----------------------------------------------------------------------
// Basic NP types
// -----------------------------------------------------------------------

#define SCE_NP_ONLINEID_MAX_LENGTH   16
#define SCE_NP_COMMUNICATION_ID_LEN  9
#define SCE_NP_TITLE_ID_LEN          9
#define SCE_NP_COMMUNICATION_PASSPHRASE_LEN 128
#define SCE_NP_TITLE_SECRET_SIZE     128

typedef struct SceNpOnlineId {
    char     data[SCE_NP_ONLINEID_MAX_LENGTH];
    char     term;
    char     dummy[3];
} SceNpOnlineId;

typedef struct SceNpId {
    SceNpOnlineId handle;
    uint8_t       opt[8];
    uint8_t       reserved[8];
} SceNpId;

typedef struct SceNpCommunicationId {
    char     data[SCE_NP_COMMUNICATION_ID_LEN];
    char     term;
    uint8_t  num;
    uint8_t  dummy;
} SceNpCommunicationId;

typedef struct SceNpCommunicationPassphrase {
    uint8_t data[SCE_NP_COMMUNICATION_PASSPHRASE_LEN];
} SceNpCommunicationPassphrase;

typedef struct SceNpCommunicationSignature {
    uint8_t data[SCE_NP_COMMUNICATION_PASSPHRASE_LEN];
} SceNpCommunicationSignature;

typedef struct SceNpCommunicationConfig {
    const SceNpCommunicationId         *commId;
    const SceNpCommunicationPassphrase *commPassphrase;
    const SceNpCommunicationSignature  *commSignature;
} SceNpCommunicationConfig;

typedef struct SceNpTitleId {
    char data[SCE_NP_TITLE_ID_LEN];
    char term;
    char dummy[3];
} SceNpTitleId;

typedef struct SceNpTitleSecret {
    uint8_t data[SCE_NP_TITLE_SECRET_SIZE];
} SceNpTitleSecret;

typedef SceNpCommunicationId SceNpCommsId;
typedef SceNpCommunicationSignature SceNpCommsSig;

// Session ID
typedef struct SceNpSessionId {
    char data[40];
    char term;
    char dummy[3];
} SceNpSessionId;

// -----------------------------------------------------------------------
// NP Service state
// -----------------------------------------------------------------------
typedef enum SceNpServiceState {
    SCE_NP_SERVICE_STATE_UNKNOWN          = 0,
    SCE_NP_SERVICE_STATE_SIGNED_OUT       = 1,
    SCE_NP_SERVICE_STATE_SIGNED_IN        = 2,
    SCE_NP_SERVICE_STATE_ONLINE           = 3
} SceNpServiceState;

// -----------------------------------------------------------------------
// NP Basic (friends, presence)
// -----------------------------------------------------------------------
typedef struct SceNpBasicGamePresence {
    uint32_t size;
    uint32_t gameStatus;
    char     data[64];
    uint32_t datalen;
} SceNpBasicGamePresence;

typedef struct SceNpBasicFriendContextState {
    uint32_t state;
} SceNpBasicFriendContextState;

typedef struct SceNpBasicAttachmentDataId {
    uint32_t id;
} SceNpBasicAttachmentDataId;

typedef struct SceNpBasicExtendedAttachmentData {
    uint32_t flags;
    uint32_t id;
    void    *data;
    uint32_t dataSize;
    uint32_t msgId;
} SceNpBasicExtendedAttachmentData;

typedef void (*SceNpBasicEventHandler)(int event, int retCode,
                                        uint32_t reqId, void *arg);
typedef void (*SceNpManagerCallback)(int event, int retCode, void *arg);

// -----------------------------------------------------------------------
// NP Matching 2
// -----------------------------------------------------------------------
typedef uint32_t SceNpMatching2ContextId;
typedef uint64_t SceNpMatching2RoomId;
typedef uint16_t SceNpMatching2ServerId;
typedef uint16_t SceNpMatching2RoomMemberId;
typedef uint32_t SceNpMatching2RequestId;
typedef uint16_t SceNpMatching2WorldId;
typedef uint32_t SceNpMatching2EventKey;
typedef uint32_t SceNpMatching2Event;
typedef uint32_t SceNpMatching2EventCause;

// -----------------------------------------------------------------------
// NP Score / Commerce
// -----------------------------------------------------------------------
typedef struct SceNpScoreComment {
    char utf8comment[64];
} SceNpScoreComment;

typedef struct SceNpScoreRankData {
    SceNpId  npId;
    uint64_t score;
    int32_t  serialRank;
    int32_t  rank;
    int32_t  highestRank;
    int32_t  _reserved;
} SceNpScoreRankData;

typedef struct SceNpCommerce2ContentRatingDescriptor {
    uint32_t id;
    char     desc[64];
} SceNpCommerce2ContentRatingDescriptor;

typedef struct SceNpCommerce2SessionInfo {
    uint32_t size;
    uint8_t  data[256];
} SceNpCommerce2SessionInfo;

// -----------------------------------------------------------------------
// User info
// -----------------------------------------------------------------------
typedef struct SceNpUserInfo {
    SceNpId userId;
    char    name[64];
} SceNpUserInfo;

#ifdef __cplusplus
extern "C" {
#endif

int  sceNpInit(const SceNpCommunicationConfig *conf, void *opt);
int  sceNpTerm(void);
int  sceNpGetServiceState(SceNpServiceState *state);
int  sceNpManagerRegisterCallback(SceNpManagerCallback callback, void *arg);
int  sceNpManagerUnregisterCallback(void);
int  sceNpBasicGetFriendListEntryCount(uint32_t *count);
int  sceNpBasicGetFriendListEntries(uint32_t startIndex,
                                     SceNpId *entries,
                                     uint32_t maxEntries,
                                     uint32_t *retrieved);
int  sceNpBasicGetGamePresenceOfFriend(const SceNpId *npId,
                                        SceNpBasicGamePresence *presence);
int  sceNpBasicGetFriendContextState(const SceNpId *npId,
                                      SceNpBasicFriendContextState *state);

#ifdef __cplusplus
}
#endif
