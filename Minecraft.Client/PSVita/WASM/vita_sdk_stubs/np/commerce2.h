#pragma once
#include <np.h>

/* NP Commerce2 stubs */
#define SCE_NP_COMMERCE2_CATEGORY_ID_LEN 56
#define SCE_NP_COMMERCE2_PRODUCT_ID_LEN 48
#define SCE_NP_COMMERCE2_CATEGORY_NAME_LEN 256
#define SCE_NP_COMMERCE2_CATEGORY_DESCRIPTION_LEN 512
#define SCE_NP_COMMERCE2_URL_LEN 256
#define SCE_NP_COMMERCE2_PRODUCT_NAME_LEN 256

#ifdef __cplusplus
extern "C" {
#endif

int sceNpCommerce2Init(void);
int sceNpCommerce2Term(void);
int sceNpCommerce2CreateCtx(uint32_t titleId, const SceNpCommunicationId *commId, void *reserved);
int sceNpCommerce2DestroyCtx(int ctx);

#ifdef __cplusplus
}
#endif
