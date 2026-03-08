#pragma once

#define SCE_NP_ONLINEID_MAX_LENGTH 16
#define SCE_NET_ETHER_ADDR_LEN     6

typedef struct SceNpOnlineId {
    char data[16];
    char term;
    char dummy[3];
} SceNpOnlineId;

typedef struct SceNpId {
    SceNpOnlineId handle;
    unsigned char opt[8];
    unsigned char reserved[8];
} SceNpId;

typedef struct SceNpTitleId {
    char id[12];
    char term;
} SceNpTitleId;

typedef struct SceNpCommunicationId {
    char data[9];
    char term;
    int num;
    int opt;
} SceNpCommunicationId;

typedef struct SceNpCommunicationPassphrase {
    unsigned char data[128];
} SceNpCommunicationPassphrase;

typedef struct SceNpCommunicationSignature {
    unsigned char data[160];
} SceNpCommunicationSignature;

typedef struct SceNpCommunicationConfig {
    const SceNpCommunicationId *id;
    const SceNpCommunicationPassphrase *passphrase;
    const SceNpCommunicationSignature *signature;
} SceNpCommunicationConfig;
