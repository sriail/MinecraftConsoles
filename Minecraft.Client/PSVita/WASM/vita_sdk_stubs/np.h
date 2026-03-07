#pragma once
/*
 * Minimal PS Vita NP SDK stubs for Emscripten/WASM builds.
 * These stubs provide just enough type and constant definitions to allow
 * the PSVita source tree to compile under Emscripten without the real SDK.
 */

#include <stdint.h>

/* -------------------------------------------------------------------------
 * General
 * ---------------------------------------------------------------------- */
#define SCE_OK                          0

/* -------------------------------------------------------------------------
 * NP Online ID
 * ---------------------------------------------------------------------- */
#define SCE_NP_ONLINEID_MAX_LENGTH      16

typedef struct SceNpOnlineId {
    char data[SCE_NP_ONLINEID_MAX_LENGTH];
    char term;
    char dummy[3];
} SceNpOnlineId;

/* -------------------------------------------------------------------------
 * NP ID (full player identity)
 * ---------------------------------------------------------------------- */
typedef struct SceNpId {
    SceNpOnlineId handle;
    uint8_t       opt[8];
    uint8_t       unk[8];
} SceNpId;

/* -------------------------------------------------------------------------
 * Communication / Title identifiers
 * ---------------------------------------------------------------------- */
#define SCE_NP_COMMUNICATIONID_LEN      9

typedef struct SceNpCommunicationId {
    char    data[SCE_NP_COMMUNICATIONID_LEN];
    char    term;
    uint8_t num;
    uint8_t dummy;
} SceNpCommunicationId;

/* Older SDK alias */
typedef SceNpCommunicationId SceNpCommsId;

/* PSP2/PSVita title ID is the same layout as communication ID */
typedef SceNpCommunicationId SceNpTitleId;

typedef struct SceNpCommunicationSignature {
    uint8_t data[160];
} SceNpCommunicationSignature;

/* Older SDK alias */
typedef SceNpCommunicationSignature SceNpCommsSig;

typedef struct SceNpCommunicationPassphrase {
    uint8_t data[128];
} SceNpCommunicationPassphrase;

typedef struct SceNpCommunicationConfig {
    const SceNpCommunicationId         *commId;
    const SceNpCommunicationPassphrase *commPassphrase;
    const SceNpCommunicationSignature  *commSignature;
} SceNpCommunicationConfig;

typedef struct SceNpTitleSecret {
    uint8_t data[40];
} SceNpTitleSecret;

/* -------------------------------------------------------------------------
 * Matching2 types
 * ---------------------------------------------------------------------- */
typedef uint64_t SceNpMatching2RoomId;
typedef uint16_t SceNpMatching2ServerId;
typedef uint16_t SceNpMatching2RoomMemberId;  /* 16-bit per SDK spec */
typedef uint32_t SceNpMatching2WorldId;
typedef uint32_t SceNpMatching2RequestId;
typedef uint32_t SceNpMatching2ContextId;
typedef uint16_t SceNpMatching2Event;
typedef uint16_t SceNpMatching2EventCause;

/* -------------------------------------------------------------------------
 * Score / leaderboard types
 * ---------------------------------------------------------------------- */
typedef struct SceNpScoreComment {
    char utf8comment[64];
} SceNpScoreComment;

/* -------------------------------------------------------------------------
 * AppUtil NP types
 * ---------------------------------------------------------------------- */
typedef struct SceAppUtilAppEventParam {
    uint32_t type;
    uint8_t  dat[1024];
} SceAppUtilAppEventParam;

typedef struct SceAppUtilNpBasicJoinablePresenceParam {
    uint32_t np2pid;
    uint8_t  data[1024];
} SceAppUtilNpBasicJoinablePresenceParam;
