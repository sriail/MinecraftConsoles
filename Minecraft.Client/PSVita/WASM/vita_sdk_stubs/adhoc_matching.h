#pragma once
#include <stdint.h>

/* adhoc_matching.h - PS Vita adhoc matching stubs */

#ifdef __cplusplus
extern "C" {
#endif

int sceNetAdhocMatchingInit(int memsize);
int sceNetAdhocMatchingTerm(void);

#ifdef __cplusplus
}
#endif
