#pragma once
#include <stdint.h>
#include <stddef.h>

#define SCE_NP_ONLINEID_MAX_LENGTH 16
#define SCE_NP_COMMUNICATION_ID_MAX_LENGTH 12
#define SCE_NP_COMMUNICATION_PASSPHRASE_SIZE 128
#define SCE_NP_COMMUNICATION_SIGNATURE_SIZE 160

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

typedef struct SceNpCommunicationId {
    char data[9];
    char term;
    uint8_t num;
    char dummy;
} SceNpCommunicationId;

typedef struct SceNpCommunicationPassphrase {
    uint8_t data[SCE_NP_COMMUNICATION_PASSPHRASE_SIZE];
} SceNpCommunicationPassphrase;

typedef struct SceNpCommunicationSignature {
    uint8_t data[SCE_NP_COMMUNICATION_SIGNATURE_SIZE];
} SceNpCommunicationSignature;

typedef struct SceNpCommunicationConfig {
    const SceNpCommunicationId *commId;
    const SceNpCommunicationPassphrase *commPassphrase;
    const SceNpCommunicationSignature *commSignature;
} SceNpCommunicationConfig;

typedef uint64_t SceNpMatching2RoomId;
typedef uint16_t SceNpMatching2ServerId;
