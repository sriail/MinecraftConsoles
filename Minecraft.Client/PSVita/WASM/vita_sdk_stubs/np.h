#pragma once
/* np.h — PS Vita Network Platform stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <string.h>
#include "np/np_common.h"
#include "np/np_basic.h"
#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Score / Comment ────────────────────────────────────────── */
#define SCE_NP_SCORE_COMMENT_MAXLEN     64
typedef struct { char str[SCE_NP_SCORE_COMMENT_MAXLEN + 1]; } SceNpScoreComment;

/* ── Matching2 types ─────────────────────────────────────────── */
typedef uint32_t SceNpMatching2ContextId;
typedef uint64_t SceNpMatching2RoomId;
typedef uint16_t SceNpMatching2RoomMemberId;
typedef uint16_t SceNpMatching2ServerId;
typedef uint32_t SceNpMatching2WorldId;
typedef uint32_t SceNpMatching2RequestId;
typedef uint16_t SceNpMatching2AttributeId;

/* Matching2 event/cause */
#define SCE_NP_MATCHING2_CONTEXT_EVENT_Start     0x1000
#define SCE_NP_MATCHING2_CONTEXT_EVENT_STARTED   0x1001
#define SCE_NP_MATCHING2_CONTEXT_EVENT_STOPPED   0x1002
#define SCE_NP_MATCHING2_CONTEXT_EVENT_START_OVER 0x1003

#define SCE_NP_MATCHING2_REQUEST_EVENT_GET_WORLD_INFO_LIST          0x2001
#define SCE_NP_MATCHING2_REQUEST_EVENT_CREATE_JOIN_ROOM             0x2002
#define SCE_NP_MATCHING2_REQUEST_EVENT_JOIN_ROOM                    0x2003
#define SCE_NP_MATCHING2_REQUEST_EVENT_LEAVE_ROOM                   0x2004
#define SCE_NP_MATCHING2_REQUEST_EVENT_GET_ROOM_DATA_EXTERNAL_LIST  0x2005
#define SCE_NP_MATCHING2_REQUEST_EVENT_GET_ROOM_MEMBER_DATA_INTERNAL 0x2006
#define SCE_NP_MATCHING2_REQUEST_EVENT_SET_ROOM_DATA_EXTERNAL       0x2007
#define SCE_NP_MATCHING2_REQUEST_EVENT_SetRoomDataExternal          SCE_NP_MATCHING2_REQUEST_EVENT_SET_ROOM_DATA_EXTERNAL

#define SCE_NP_MATCHING2_ROOM_EVENT_MEMBER_JOINED           0x3001
#define SCE_NP_MATCHING2_ROOM_EVENT_MEMBER_LEFT             0x3002
#define SCE_NP_MATCHING2_ROOM_EVENT_KICKEDOUT               0x3003
#define SCE_NP_MATCHING2_ROOM_EVENT_Kickedout               SCE_NP_MATCHING2_ROOM_EVENT_KICKEDOUT
#define SCE_NP_MATCHING2_ROOM_EVENT_ROOM_OWNER_CHANGED      0x3004
#define SCE_NP_MATCHING2_ROOM_EVENT_UPDATED_ROOM_DATA_INTERNAL  0x3005
#define SCE_NP_MATCHING2_ROOM_EVENT_UPDATED_ROOM_MEMBER_DATA_INTERNAL 0x3006
#define SCE_NP_MATCHING2_ROOM_EVENT_ROOM_DESTROYED          0x3007
#define SCE_NP_MATCHING2_ROOM_EVENT_RoomDestroyed           SCE_NP_MATCHING2_ROOM_EVENT_ROOM_DESTROYED
#define SCE_NP_MATCHING2_ROOM_EVENT_UPDATED_SIGNALING_OPT_PARAM 0x3008

#define SCE_NP_MATCHING2_SIGNALING_EVENT_ESTABLISHED        0x4001
#define SCE_NP_MATCHING2_SIGNALING_EVENT_DEAD               0x4002

typedef int SceNpMatching2Event;
typedef int SceNpMatching2EventCause;
typedef int SceNpMatching2EventKey;

#define SCE_NP_MATCHING2_EVENT_CAUSE_NP_SIGNED_OUT  1

/* Error codes */
#define SCE_NP_MATCHING2_ERROR_CONTEXT_NOT_STARTED   -2137063423
#define SCE_NP_MATCHING2_ERROR_CONTEXT_UNAVAILABLE   -2137063422
#define SCE_NP_MATCHING2_ERROR_NP_SIGNED_OUT         -2137063421
#define SCE_NP_MATCHING2_SERVER_ERROR_ROOM_FULL      -2137059071
#define SCE_NP_MATCHING2_SERVER_ERROR_NAT_TYPE_MISMATCH -2137059070
#define SCE_NP_MATCHING2_SERVER_STATUS_AVAILABLE     1

/* Attribute sizes */
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_EXTERNAL_1_ID        0x004A
#define SCE_NP_MATCHING2_ROOM_BIN_ATTR_INTERNAL_1_ID        0x0001
#define SCE_NP_MATCHING2_ROOMMEMBER_BIN_ATTR_INTERNAL_1_ID  0x0001

#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomUpdateInfo         512
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomDataInternalUpdateInfo 1024
#define SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomMemberDataInternalUpdateInfo 512

/* Room flags */
#define SCE_NP_MATCHING2_ROOM_FLAG_ATTR_NAT_TYPE_RESTRICTION  0x0001

/* Signaling types */
#define SCE_NP_MATCHING2_SIGNALING_TYPE_MESH  1

/* Binary attribute */
typedef struct {
    SceNpMatching2AttributeId id;
    uint8_t                   pad[2];
    uint32_t                  size;
    void                     *data;
} SceNpMatching2BinAttr;

/* Signaling opt param */
typedef struct {
    uint8_t  type;
    uint8_t  pad[3];
    uint32_t flag;
    uint32_t natType;
} SceNpMatching2SignalingOptParam;

/* Signaling net info */
typedef struct {
    SceNetInAddr  mapped;
    SceNetInAddr  local;
    uint16_t      mappedPort;
    uint16_t      localPort;
    uint8_t       natType;
    uint8_t       pad[3];
} SceNpMatching2SignalingNetInfo;

/* Room data external */
typedef struct {
    SceNpMatching2RoomId     roomId;
    SceNpMatching2WorldId    worldId;
    SceNpMatching2ServerId   serverId;
    uint16_t                 maxSlot;
    uint16_t                 curMemberNum;
    uint8_t                  pad[4];
    SceNpMatching2BinAttr   *roomBinAttrExternal;
    uint32_t                 roomBinAttrExternalNum;
} SceNpMatching2RoomDataExternal;

/* Room update info */
typedef struct {
    uint8_t data[SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomUpdateInfo];
} SceNpMatching2RoomUpdateInfo;

/* Room data internal update info */
typedef struct {
    uint8_t data[SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomDataInternalUpdateInfo];
} SceNpMatching2RoomDataInternalUpdateInfo;

/* Room member data internal update info */
typedef struct {
    uint8_t data[SCE_NP_MATCHING2_EVENT_DATA_MAX_SIZE_RoomMemberDataInternalUpdateInfo];
} SceNpMatching2RoomMemberDataInternalUpdateInfo;

/* Server struct */
typedef struct {
    SceNpMatching2ServerId serverId;
    uint16_t               worldNum;
    uint16_t               pad;
} SceNpMatching2Server;

/* Request callbacks */
typedef void (*SceNpMatching2RequestCallback)(SceNpMatching2ContextId ctxId,
    SceNpMatching2RequestId reqId, SceNpMatching2Event event,
    SceNpMatching2EventCause eventCause, int errorCode,
    void *pData, size_t dataLen, void *arg);

typedef void (*SceNpMatching2ContextCallback)(SceNpMatching2ContextId ctxId,
    SceNpMatching2Event event, SceNpMatching2EventCause eventCause,
    int errorCode, void *pData, size_t dataLen, void *arg);

typedef void (*SceNpMatching2SignalingCallback)(SceNpMatching2ContextId ctxId,
    SceNpMatching2RoomId roomId, SceNpMatching2RoomMemberId memberId,
    SceNpMatching2Event event, int errorCode, void *arg);

/* Request opt param */
typedef struct {
    size_t      size;
    SceNpId    *npId;
    uint64_t    timeout;
} SceNpMatching2RequestOptParam;

/* Get world info list request/response */
typedef struct {
    SceNpMatching2ServerId serverId;
    uint16_t               pad;
    uint32_t               worldNum;
} SceNpMatching2GetWorldInfoListRequest;

typedef struct {
    SceNpMatching2Server *world;
    uint32_t              worldNum;
} SceNpMatching2GetWorldInfoListResponse;

/* Create/join room request/response */
typedef struct {
    SceNpMatching2WorldId     worldId;
    SceNpMatching2ServerId    serverId;
    uint16_t                  maxSlot;
    uint8_t                   pad[4];
    SceNpMatching2BinAttr    *roomBinAttrExternal;
    uint32_t                  roomBinAttrExternalNum;
    SceNpMatching2BinAttr    *roomBinAttrInternal;
    uint32_t                  roomBinAttrInternalNum;
    SceNpMatching2BinAttr    *roomMemberBinAttrInternal;
    uint32_t                  roomMemberBinAttrInternalNum;
    SceNpMatching2SignalingOptParam *signalingOptParam;
    uint32_t                  roomFlag;
    uint32_t                  flagFilter;
} SceNpMatching2CreateJoinRoomRequest;

typedef struct {
    SceNpMatching2RoomId     roomId;
    SceNpMatching2RoomMemberId memberId;
    uint16_t                  pad;
} SceNpMatching2CreateJoinRoomResponse;

/* Join room */
typedef struct {
    SceNpMatching2RoomId    roomId;
    SceNpMatching2ServerId  serverId;
    uint16_t                pad;
    SceNpMatching2BinAttr  *roomMemberBinAttrInternal;
    uint32_t                roomMemberBinAttrInternalNum;
} SceNpMatching2JoinRoomRequest;

typedef SceNpMatching2CreateJoinRoomResponse SceNpMatching2JoinRoomResponse;

/* Leave room */
typedef struct {
    SceNpMatching2RoomId roomId;
    uint32_t             optData;
} SceNpMatching2LeaveRoomRequest;

/* Kickout */
typedef struct {
    SceNpMatching2RoomId       roomId;
    SceNpMatching2RoomMemberId memberId;
    uint16_t                   pad;
    uint32_t                   optData;
} SceNpMatching2KickoutRoomMemberRequest;

/* Set room data external */
typedef struct {
    SceNpMatching2RoomId    roomId;
    SceNpMatching2ServerId  serverId;
    uint16_t                pad;
    SceNpMatching2BinAttr  *roomBinAttrExternal;
    uint32_t                roomBinAttrExternalNum;
} SceNpMatching2SetRoomDataExternalRequest;

/* Set room data internal */
typedef struct {
    SceNpMatching2RoomId    roomId;
    SceNpMatching2BinAttr  *roomBinAttrInternal;
    uint32_t                roomBinAttrInternalNum;
    uint8_t                 pad[4];
} SceNpMatching2SetRoomDataInternalRequest;

/* Set room member data internal */
typedef struct {
    SceNpMatching2RoomId       roomId;
    SceNpMatching2RoomMemberId memberId;
    uint16_t                   pad;
    SceNpMatching2BinAttr     *roomMemberBinAttrInternal;
    uint32_t                   roomMemberBinAttrInternalNum;
} SceNpMatching2SetRoomMemberDataInternalRequest;

/* Get room data external list */
typedef struct {
    SceNpMatching2ServerId  serverId;
    SceNpMatching2WorldId   worldId;
    uint32_t                startIndex;
    uint32_t                count;
} SceNpMatching2GetRoomDataExternalListRequest;

typedef struct {
    SceNpMatching2RoomDataExternal *rooms;
    uint32_t                        roomNum;
} SceNpMatching2GetRoomDataExternalListResponse;

/* Get room member data internal */
typedef struct {
    SceNpMatching2RoomId       roomId;
    SceNpMatching2RoomMemberId memberId;
    uint16_t                   pad;
    SceNpMatching2AttributeId *memberBinAttrIds;
    uint32_t                   memberBinAttrNum;
} SceNpMatching2GetRoomMemberDataInternalRequest;

typedef struct {
    uint8_t data[256];
} SceNpMatching2GetRoomMemberDataInternalResponse;

/* NP Manager callback */
typedef void (*SceNpManagerCallback)(int event, int result, void *arg);

/* Auth */
typedef struct {
    uint32_t    size;
    SceNpId    *npId;
    SceNpClientId *clientId;
    const char  *scope;
    uint32_t    flag;
} SceNpAuthGetAuthorizationCodeParameter;

/* NP Commerce2 session info */
typedef struct { uint8_t data[64]; } SceNpCommerce2SessionInfo;

/* ── NP Core API stubs ──────────────────────────────────────── */
static inline int sceNpInit(const SceNpCommunicationConfig *config, void *opt)
{ (void)config; (void)opt; return SCE_OK; }

static inline int sceNpTerm(void) { return SCE_OK; }

static inline int sceNpGetNpId(int userId, SceNpId *npId)
{ (void)userId; if (npId) memset(npId, 0, sizeof(*npId)); return SCE_OK; }

static inline int sceNpGetServiceState(SceNpServiceState *pState)
{ if (pState) *pState = SCE_NP_SERVICE_STATE_ONLINE; return SCE_OK; }

static inline int sceNpRegisterServiceStateCallback(SceNpManagerCallback cb, void *arg)
{ (void)cb; (void)arg; return SCE_OK; }

static inline int sceNpUnregisterServiceStateCallback(void) { return SCE_OK; }

static inline int sceNpCheckNpAvailability(void *opt) { (void)opt; return SCE_OK; }

/* Score stubs */
static inline int sceNpScoreInit(int threadPriority, int cpuAffinityMask, void *opt)
{ (void)threadPriority; (void)cpuAffinityMask; (void)opt; return SCE_OK; }

static inline int sceNpScoreTerm(void) { return SCE_OK; }

static inline int sceNpScoreCreateTitleCtx(const SceNpCommunicationId *commId,
    const SceNpCommunicationPassphrase *passphrase, const SceNpId *selfNpId)
{ (void)commId; (void)passphrase; (void)selfNpId; return 1; }

static inline int sceNpScoreDeleteTitleCtx(int titleCtxId)
{ (void)titleCtxId; return SCE_OK; }

static inline int sceNpScoreGetBoardInfo(int titleCtxId, unsigned int boardId,
    void *pBoardInfo, void *opt)
{ (void)titleCtxId; (void)boardId; (void)pBoardInfo; (void)opt; return SCE_OK; }

static inline int sceNpScoreRecordScore(int titleCtxId, unsigned int boardId,
    int64_t score, const SceNpScoreComment *comment, void *reserved,
    int64_t *pOutRecordedScore, void *opt)
{
    (void)titleCtxId; (void)boardId; (void)score; (void)comment;
    (void)reserved; (void)opt;
    if (pOutRecordedScore) *pOutRecordedScore = score;
    return SCE_OK;
}

/* Matching2 stubs */
static inline int sceNpMatching2Init(size_t poolSize, void *opt)
{ (void)poolSize; (void)opt; return SCE_OK; }

static inline int sceNpMatching2Term(void) { return SCE_OK; }

static inline int sceNpMatching2CreateContext(const SceNpId *selfNpId,
    const SceNpCommunicationId *commId,
    const SceNpCommunicationPassphrase *passphrase,
    SceNpMatching2ContextId *ctxId, void *opt)
{
    (void)selfNpId; (void)commId; (void)passphrase; (void)opt;
    if (ctxId) *ctxId = 1;
    return SCE_OK;
}

static inline int sceNpMatching2DestroyContext(SceNpMatching2ContextId ctxId)
{ (void)ctxId; return SCE_OK; }

static inline int sceNpMatching2StartContext(SceNpMatching2ContextId ctxId,
    SceNpMatching2ContextCallback cb, void *arg,
    SceNpMatching2EventKey *pEventKey)
{
    (void)ctxId; (void)cb; (void)arg; (void)pEventKey;
    return SCE_OK;
}

static inline int sceNpMatching2StopContext(SceNpMatching2ContextId ctxId,
    SceNpMatching2EventKey *pEventKey)
{ (void)ctxId; (void)pEventKey; return SCE_OK; }

static inline int sceNpMatching2GetWorldInfoList(SceNpMatching2ContextId ctxId,
    const SceNpMatching2GetWorldInfoListRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2CreateJoinRoom(SceNpMatching2ContextId ctxId,
    const SceNpMatching2CreateJoinRoomRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2JoinRoom(SceNpMatching2ContextId ctxId,
    const SceNpMatching2JoinRoomRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2LeaveRoom(SceNpMatching2ContextId ctxId,
    const SceNpMatching2LeaveRoomRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2KickoutRoomMember(SceNpMatching2ContextId ctxId,
    const SceNpMatching2KickoutRoomMemberRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2SetRoomDataExternal(SceNpMatching2ContextId ctxId,
    const SceNpMatching2SetRoomDataExternalRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2SetRoomDataInternal(SceNpMatching2ContextId ctxId,
    const SceNpMatching2SetRoomDataInternalRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2SetRoomMemberDataInternal(SceNpMatching2ContextId ctxId,
    const SceNpMatching2SetRoomMemberDataInternalRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2GetRoomDataExternalList(SceNpMatching2ContextId ctxId,
    const SceNpMatching2GetRoomDataExternalListRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2GetRoomMemberDataInternal(SceNpMatching2ContextId ctxId,
    const SceNpMatching2GetRoomMemberDataInternalRequest *pReq,
    const SceNpMatching2RequestOptParam *pOpt,
    SceNpMatching2RequestId *pReqId)
{
    (void)ctxId; (void)pReq; (void)pOpt;
    if (pReqId) *pReqId = 0;
    return SCE_OK;
}

static inline int sceNpMatching2GetEventData(SceNpMatching2ContextId ctxId,
    SceNpMatching2EventKey eventKey, void *pData, size_t dataLen)
{ (void)ctxId; (void)eventKey; (void)pData; (void)dataLen; return SCE_OK; }

static inline int sceNpMatching2AbortRequest(SceNpMatching2ContextId ctxId,
    SceNpMatching2RequestId reqId)
{ (void)ctxId; (void)reqId; return SCE_OK; }

static inline int sceNpMatching2RegisterRoomEventCallback(
    SceNpMatching2ContextId ctxId, void *cb, void *arg)
{ (void)ctxId; (void)cb; (void)arg; return SCE_OK; }

static inline int sceNpMatching2RegisterRoomMessageCallback(
    SceNpMatching2ContextId ctxId, void *cb, void *arg)
{ (void)ctxId; (void)cb; (void)arg; return SCE_OK; }

static inline int sceNpMatching2RegisterSignalingCallback(
    SceNpMatching2ContextId ctxId,
    SceNpMatching2SignalingCallback cb, void *arg)
{ (void)ctxId; (void)cb; (void)arg; return SCE_OK; }

static inline int sceNpMatching2GetSignalingOptParamLocal(
    SceNpMatching2ContextId ctxId, SceNpMatching2RoomId roomId,
    SceNpMatching2SignalingOptParam *pOpt)
{
    (void)ctxId; (void)roomId;
    if (pOpt) memset(pOpt, 0, sizeof(*pOpt));
    return SCE_OK;
}

static inline int sceNpMatching2GetSignalingNetInfo(
    SceNpMatching2ContextId ctxId, SceNpMatching2RoomId roomId,
    SceNpMatching2RoomMemberId memberId,
    SceNpMatching2SignalingNetInfo *pInfo)
{
    (void)ctxId; (void)roomId; (void)memberId;
    if (pInfo) memset(pInfo, 0, sizeof(*pInfo));
    return SCE_OK;
}

#ifdef __cplusplus
}
#endif
