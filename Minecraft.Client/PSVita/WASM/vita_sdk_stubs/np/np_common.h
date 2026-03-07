#pragma once
/* np/np_common.h — PS Vita NP common type stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <string.h>
#include "../net.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Online ID */
#define SCE_NP_ONLINEID_MAX_LENGTH  17
#define SCE_NP_ONLINEID_MIN_LENGTH   3

typedef struct {
    char  data[SCE_NP_ONLINEID_MAX_LENGTH];
    char  term;
    uint8_t dummy[2];
} SceNpOnlineId;

/* Communication ID */
#define SCE_NP_COMMUNICATION_ID_SIZE    9
#define SCE_NP_COMMUNICATION_ID_LEN     9

typedef struct {
    char     data[SCE_NP_COMMUNICATION_ID_SIZE];
    char     term;
    uint8_t  num;
    uint8_t  dummy;
} SceNpCommunicationId;

typedef SceNpCommunicationId SceNpCommsId;

/* Communication signature/passphrase */
#define SCE_NP_COMMUNICATION_PASSPHRASE_SIZE    128
#define SCE_NP_COMMUNICATION_SIGNATURE_SIZE     160

typedef struct { uint8_t data[SCE_NP_COMMUNICATION_PASSPHRASE_SIZE]; } SceNpCommunicationPassphrase;
typedef struct { uint8_t data[SCE_NP_COMMUNICATION_SIGNATURE_SIZE]; } SceNpCommunicationSignature;

typedef SceNpCommunicationSignature SceNpCommsSig;

/* Title ID / secret */
#define SCE_NP_TITLE_ID_MAX_LENGTH  12
#define SCE_NP_TITLE_SECRET_SIZE   128

typedef struct {
    char data[SCE_NP_TITLE_ID_MAX_LENGTH];
    char term;
    uint8_t dummy[3];
} SceNpTitleId;

typedef struct { uint8_t data[SCE_NP_TITLE_SECRET_SIZE]; } SceNpTitleSecret;

/* NP ID */
typedef struct {
    SceNpOnlineId  handle;
    uint8_t        opt[8];
    uint8_t        reserved[8];
} SceNpId;

/* User info */
typedef struct {
    SceNpId   npId;
    char      icon_url[512];
} SceNpUserInfo;

/* Client ID */
typedef struct { char data[37]; } SceNpClientId;

/* Authorization code */
#define SCE_NP_AUTHORIZATION_CODE_MAX_LEN  64
typedef struct { char code[SCE_NP_AUTHORIZATION_CODE_MAX_LEN]; } SceNpAuthorizationCode;

/* Entitlement */
#define SCE_NP_SERVICE_ENTITLEMENT_ID_SIZE  32
typedef struct {
    char    id[SCE_NP_SERVICE_ENTITLEMENT_ID_SIZE];
    uint8_t entitlementType;
    uint8_t category;
    uint8_t pad[2];
    int32_t count;
    int32_t remainingCount;
} SceNpEntitlement;

/* Session ID */
#define SCE_NP_SESSION_ID_MAX_SIZE  79
typedef struct { char data[SCE_NP_SESSION_ID_MAX_SIZE + 1]; } SceNpSessionId;

/* Communication config */
typedef struct {
    const SceNpCommunicationId          *commId;
    const SceNpCommunicationPassphrase  *commPassphrase;
    const SceNpCommunicationSignature   *commSignature;
} SceNpCommunicationConfig;

/* Service state */
#define SCE_NP_SERVICE_STATE_UNKNOWN    0
#define SCE_NP_SERVICE_STATE_SIGNED_OUT 1
#define SCE_NP_SERVICE_STATE_SIGNED_IN  2
#define SCE_NP_SERVICE_STATE_ONLINE     3

typedef int SceNpServiceState;

/* Manager status */
#define SCE_NP_MANAGER_STATUS_ONLINE    SCE_NP_SERVICE_STATE_ONLINE

/* Error codes */
#define SCE_NP_ERROR_ALREADY_INITIALIZED    -2141913087  /* 0xFF020001 */
#define SCE_NP_COMMUNITY_ERROR_ALREADY_INITIALIZED  -2141847551
#define SCE_NP_AUTH_ERROR_ALREADY_INITIALIZED       -2141716479

/* Port */
#define SCE_NP_PORT  3658

#ifdef __cplusplus
}
#endif
