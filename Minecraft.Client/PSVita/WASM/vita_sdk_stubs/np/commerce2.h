#pragma once

typedef int SceNpCommerceCtx;

typedef struct SceNpCommerceProductSkuInfo {
    unsigned char data[256];
} SceNpCommerceProductSkuInfo;

inline int sceNpCommerceCreateCtx(SceNpCommerceCtx *ctx) {
    (void)ctx;
    return 0;
}

inline int sceNpCommerceDestroyCtx(SceNpCommerceCtx ctx) {
    (void)ctx;
    return 0;
}

inline int sceNpCommerceGetProductSkuInfo(SceNpCommerceCtx ctx, const char *skuId, SceNpCommerceProductSkuInfo *info) {
    (void)ctx; (void)skuId; (void)info;
    return 0;
}
