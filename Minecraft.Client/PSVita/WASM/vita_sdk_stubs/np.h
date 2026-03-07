#pragma once
#include <stdint.h>
#include <stddef.h>

#ifndef SCE_OK
#define SCE_OK 0
#endif

/* ---- NP Online ID ---- */
#define SCE_NP_ONLINEID_MAX_LENGTH 16
#define SCE_NP_COMMUNICATION_ID_MAX_LENGTH 12
#define SCE_NP_COMMUNICATION_PASSPHRASE_SIZE 128
#define SCE_NP_COMMUNICATION_SIGNATURE_SIZE 160
#define SCE_NP_TITLE_ID_LEN 12
#define SCE_NP_TITLE_SECRET_SIZE 128

typedef struct SceNpOnlineId {
    char data[SCE_NP_ONLINEID_MAX_LENGTH];
    char term;
    char dummy[3];
} SceNpOnlineId;

typedef struct SceNpId {
    SceNpOnlineId handle;
    uint8_t opt[8];
    uint8_t reserved[8];
} SceNpId;

typedef struct SceNpUserInfo {
    SceNpId userId;
} SceNpUserInfo;

typedef struct SceNpCommunicationId {
    char data[9];
    char term;
    uint8_t num;
    char dummy;
} SceNpCommunicationId;

typedef SceNpCommunicationId SceNpCommsId;

typedef struct SceNpCommunicationPassphrase {
    uint8_t data[SCE_NP_COMMUNICATION_PASSPHRASE_SIZE];
} SceNpCommunicationPassphrase;

typedef SceNpCommunicationPassphrase SceNpCommsSig;

typedef struct SceNpCommunicationSignature {
    uint8_t data[SCE_NP_COMMUNICATION_SIGNATURE_SIZE];
} SceNpCommunicationSignature;

typedef struct SceNpCommunicationConfig {
    const SceNpCommunicationId *commId;
    const SceNpCommunicationPassphrase *commPassphrase;
    const SceNpCommunicationSignature *commSignature;
} SceNpCommunicationConfig;

typedef struct SceNpTitleId {
    char data[SCE_NP_TITLE_ID_LEN + 1];
} SceNpTitleId;

typedef struct SceNpTitleSecret {
    uint8_t data[SCE_NP_TITLE_SECRET_SIZE];
} SceNpTitleSecret;

typedef struct SceNpSessionId {
    uint8_t data[32];
} SceNpSessionId;

typedef struct SceNpClientId {
    char id[128];
} SceNpClientId;

typedef struct SceNpEntitlement {
    char id[32];
    uint64_t createdDate;
    uint64_t expireDate;
    uint32_t type;
    int32_t remainingCount;
    uint32_t consumedCount;
} SceNpEntitlement;

typedef struct SceNpAuthorizationCode {
    char code[128];
} SceNpAuthorizationCode;

typedef struct SceNpAuthGetAuthorizationCodeParameter {
    uint32_t size;
    const char *scope;
    const SceNpClientId *clientId;
} SceNpAuthGetAuthorizationCodeParameter;

/* ---- NP Manager ---- */
typedef int SceNpServiceState;
typedef void (*SceNpManagerCallback)(int event, int result, void *arg);

/* ---- NP Basic ---- */
typedef uint64_t SceNpBasicAttachmentDataId;
typedef void (*SceNpBasicEventHandler)(int event, int retCode, uint32_t reqId, void *arg);

typedef struct SceNpBasicGamePresence {
    uint32_t struct_size;
    int32_t  status;
    char     title[128];
    char     statusStr[192];
} SceNpBasicGamePresence;

typedef struct SceNpBasicExtendedAttachmentData {
    uint32_t flags;
    SceNpBasicAttachmentDataId dataId;
} SceNpBasicExtendedAttachmentData;

typedef int SceNpBasicFriendContextState;

/* ---- NP Score / Ranking ---- */
typedef int64_t SceNpScoreRankNumber;
typedef struct SceNpScoreComment { char utf8Comment[64]; } SceNpScoreComment;
typedef struct SceNpScoreRankData {
    SceNpId       npId;
    uint32_t      serialRank;
    int64_t       rank;
    int64_t       highestRank;
    int32_t       hasGameData;
    uint8_t       pad[4];
    int64_t       scoreValue;
    uint64_t      recordDate;
} SceNpScoreRankData;

typedef struct SceNpScorePlayerRankData {
    int32_t           hasData;
    uint8_t           pad[4];
    SceNpScoreRankData rankData;
} SceNpScorePlayerRankData;

/* ---- NP Matching2 ---- */
typedef uint64_t SceNpMatching2RoomId;
typedef uint16_t SceNpMatching2ServerId;
typedef uint16_t SceNpMatching2WorldId;
typedef uint16_t SceNpMatching2RoomMemberId;
typedef int      SceNpMatching2ContextId;
typedef int      SceNpMatching2RequestId;
typedef uint16_t SceNpMatching2AttributeId;
typedef int      SceNpMatching2Event;
typedef int      SceNpMatching2EventCause;
typedef int      SceNpMatching2EventKey;

typedef void (*SceNpMatching2ContextCallback)(SceNpMatching2ContextId id, SceNpMatching2Event event, SceNpMatching2EventCause cause, int errorCode, void *arg);
typedef void (*SceNpMatching2RequestCallback)(SceNpMatching2ContextId id, SceNpMatching2RequestId reqId, SceNpMatching2Event event, int errorCode, const void *data, void *arg);
typedef void (*SceNpMatching2SignalingCallback)(SceNpMatching2ContextId ctxId, SceNpMatching2RoomId roomId, SceNpMatching2RoomMemberId memberId, SceNpMatching2Event event, int errorCode, void *arg);

typedef struct SceNpMatching2PresenceOptionData {
    uint8_t data[16];
    size_t  len;
} SceNpMatching2PresenceOptionData;

typedef struct SceNpMatching2BinAttr {
    SceNpMatching2AttributeId id;
    void  *ptr;
    size_t size;
} SceNpMatching2BinAttr;

typedef struct SceNpMatching2RequestOptParam {
    size_t cbSize;
    SceNpMatching2RequestCallback cbFunc;
    void  *cbFuncArg;
    int    timeout;
    uint16_t appReqId;
} SceNpMatching2RequestOptParam;

typedef struct SceNpMatching2SignalingOptParam {
    size_t cbSize;
    SceNpMatching2SignalingCallback cbFunc;
    void  *cbFuncArg;
} SceNpMatching2SignalingOptParam;

typedef struct SceNpMatching2SignalingNetInfo {
    size_t   cbSize;
    uint32_t localAddr;
    uint32_t mappedAddr;
    int      natStatus;
} SceNpMatching2SignalingNetInfo;

typedef struct SceNpMatching2Server {
    SceNpMatching2ServerId serverId;
    int      status;
} SceNpMatching2Server;

typedef struct SceNpMatching2RoomDataExternal {
    SceNpMatching2RoomId   roomId;
    SceNpMatching2ServerId serverId;
    uint16_t               worldId;
    uint16_t               publicSlotNum;
    uint16_t               privateSlotNum;
    uint16_t               openPublicSlotNum;
    uint16_t               openPrivateSlotNum;
    SceNpMatching2BinAttr *searchBinAttr;
    uint32_t               searchBinAttrNum;
} SceNpMatching2RoomDataExternal;

typedef struct SceNpMatching2RoomDataInternalUpdateInfo {
    uint32_t flagFilter;
    const void *newRoomData;
} SceNpMatching2RoomDataInternalUpdateInfo;

typedef struct SceNpMatching2RoomMemberDataInternalUpdateInfo {
    SceNpMatching2RoomMemberId memberId;
    uint32_t flagFilter;
    const void *newMemberData;
} SceNpMatching2RoomMemberDataInternalUpdateInfo;

typedef struct SceNpMatching2RoomUpdateInfo {
    int eventCause;
    int errorCode;
    SceNpMatching2RoomMemberId optMemberId;
} SceNpMatching2RoomUpdateInfo;

typedef struct SceNpMatching2GetWorldInfoListRequest {
    SceNpMatching2ServerId serverId;
} SceNpMatching2GetWorldInfoListRequest;

typedef struct SceNpMatching2GetWorldInfoListResponse {
    uint32_t worldNum;
    void    *world;
} SceNpMatching2GetWorldInfoListResponse;

typedef struct SceNpMatching2CreateJoinRoomRequest {
    SceNpMatching2ServerId serverId;
    SceNpMatching2WorldId  worldId;
    uint32_t lobbyId;
    uint32_t maxSlot;
    uint32_t flagAttr;
    SceNpMatching2BinAttr *roomBinAttrInternal;
    uint32_t roomBinAttrInternalNum;
    SceNpMatching2BinAttr *roomSearchableBinAttrExternal;
    uint32_t roomSearchableBinAttrExternalNum;
    SceNpMatching2BinAttr *roomBinAttrExternal;
    uint32_t roomBinAttrExternalNum;
    SceNpMatching2PresenceOptionData *roomGroupPresetData;
    uint32_t roomGroupPresetDataNum;
    SceNpMatching2BinAttr *roomMemberBinAttrInternal;
    uint32_t roomMemberBinAttrInternalNum;
    uint32_t teamId;
    SceNpMatching2SignalingOptParam *sigOptParam;
} SceNpMatching2CreateJoinRoomRequest;

typedef struct SceNpMatching2CreateJoinRoomResponse {
    SceNpMatching2RoomId roomId;
} SceNpMatching2CreateJoinRoomResponse;

typedef struct SceNpMatching2JoinRoomRequest {
    SceNpMatching2RoomId roomId;
    SceNpMatching2PresenceOptionData *optData;
    SceNpMatching2BinAttr *roomMemberBinAttrInternal;
    uint32_t roomMemberBinAttrInternalNum;
} SceNpMatching2JoinRoomRequest;

typedef struct SceNpMatching2JoinRoomResponse {
    SceNpMatching2RoomId roomId;
} SceNpMatching2JoinRoomResponse;

typedef struct SceNpMatching2LeaveRoomRequest {
    SceNpMatching2RoomId roomId;
    SceNpMatching2PresenceOptionData *optData;
} SceNpMatching2LeaveRoomRequest;

typedef struct SceNpMatching2KickoutRoomMemberRequest {
    SceNpMatching2RoomId roomId;
    SceNpMatching2RoomMemberId memberId;
    int blockKickFlag;
} SceNpMatching2KickoutRoomMemberRequest;

typedef struct SceNpMatching2SetRoomDataInternalRequest {
    SceNpMatching2RoomId roomId;
    uint32_t flagFilter;
    SceNpMatching2BinAttr *roomBinAttrInternal;
    uint32_t roomBinAttrInternalNum;
} SceNpMatching2SetRoomDataInternalRequest;

typedef struct SceNpMatching2SetRoomDataExternalRequest {
    SceNpMatching2RoomId roomId;
    SceNpMatching2BinAttr *roomSearchableBinAttrExternal;
    uint32_t roomSearchableBinAttrExternalNum;
    SceNpMatching2BinAttr *roomBinAttrExternal;
    uint32_t roomBinAttrExternalNum;
} SceNpMatching2SetRoomDataExternalRequest;

typedef struct SceNpMatching2SetRoomMemberDataInternalRequest {
    SceNpMatching2RoomId roomId;
    SceNpMatching2RoomMemberId memberId;
    uint32_t teamId;
    uint32_t flagFilter;
    SceNpMatching2BinAttr *roomMemberBinAttrInternal;
    uint32_t roomMemberBinAttrInternalNum;
} SceNpMatching2SetRoomMemberDataInternalRequest;

typedef struct SceNpMatching2GetRoomDataExternalListRequest {
    SceNpMatching2RoomId *roomId;
    uint32_t roomIdNum;
    const SceNpMatching2AttributeId *attrId;
    uint32_t attrIdNum;
} SceNpMatching2GetRoomDataExternalListRequest;

typedef struct SceNpMatching2GetRoomDataExternalListResponse {
    SceNpMatching2RoomDataExternal *roomDataExternal;
    uint32_t roomDataExternalNum;
} SceNpMatching2GetRoomDataExternalListResponse;

typedef struct SceNpMatching2GetRoomMemberDataInternalRequest {
    SceNpMatching2RoomId roomId;
    SceNpMatching2RoomMemberId memberId;
    const SceNpMatching2AttributeId *attrId;
    uint32_t attrIdNum;
} SceNpMatching2GetRoomMemberDataInternalRequest;

typedef struct SceNpMatching2GetRoomMemberDataInternalResponse {
    void *roomMemberDataInternal;
} SceNpMatching2GetRoomMemberDataInternalResponse;

/* ---- NP Commerce2 ---- */
typedef struct SceNpCommerce2SessionInfo { int dummy; } SceNpCommerce2SessionInfo;
typedef struct SceNpCommerce2CategoryInfo { char categoryId[64]; char categoryName[256]; uint32_t countOfSubCategory; uint32_t countOfProduct; } SceNpCommerce2CategoryInfo;
typedef struct SceNpCommerce2ContentInfo { char entitlementId[48]; } SceNpCommerce2ContentInfo;
typedef struct SceNpCommerce2ContentRatingDescriptor { uint8_t data[128]; } SceNpCommerce2ContentRatingDescriptor;
typedef struct SceNpCommerce2ContentRatingInfo { int32_t count; SceNpCommerce2ContentRatingDescriptor desc[8]; } SceNpCommerce2ContentRatingInfo;
typedef struct SceNpCommerce2GameProductInfo { char productId[48]; char productName[256]; } SceNpCommerce2GameProductInfo;
typedef struct SceNpCommerce2GameSkuInfo { char skuId[48]; uint32_t price; char displayPrice[64]; } SceNpCommerce2GameSkuInfo;
typedef struct SceNpCommerce2GetCategoryContentsResult {
    SceNpCommerce2CategoryInfo *categoryInfo;
    SceNpCommerce2GameProductInfo *productInfo;
    uint32_t categoryNum;
    uint32_t productNum;
} SceNpCommerce2GetCategoryContentsResult;
typedef struct SceNpCommerce2GetProductInfoResult {
    SceNpCommerce2GameProductInfo productInfo;
    SceNpCommerce2GameSkuInfo *skuInfo;
    uint32_t skuNum;
} SceNpCommerce2GetProductInfoResult;

/* ---- NP Commerce Dialog (also in np_commerce_dialog.h) ---- */
#ifndef _VITA_SDK_STUB_NP_COMMERCE_DIALOG_TYPES
#define _VITA_SDK_STUB_NP_COMMERCE_DIALOG_TYPES
typedef struct SceNpCommerceDialogParam { uint32_t size; } SceNpCommerceDialogParam;
typedef struct SceNpCommerceDialogResult { int32_t result; } SceNpCommerceDialogResult;
#endif
