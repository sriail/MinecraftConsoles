#pragma once
/* libperf.h — PS Vita performance counter stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t ScePerfArmPmcCounter;

#define SCE_PERF_ARM_PMC_COUNTER_0 0
#define SCE_PERF_ARM_PMC_COUNTER_1 1
#define SCE_PERF_ARM_PMC_COUNTER_2 2
#define SCE_PERF_ARM_PMC_COUNTER_3 3
#define SCE_PERF_ARM_PMC_COUNTER_4 4
#define SCE_PERF_ARM_PMC_COUNTER_5 5

#define SCE_PERF_ARM_PMC_CONFIG_CYCLE_COUNT    0xFF

static inline int scePerfArmPmcGetAndClearOverflowStatus(uint32_t *pStatus)
{ (void)pStatus; return SCE_OK; }

static inline int scePerfArmPmcEnableCounter(ScePerfArmPmcCounter counterIdx,
    int enable)
{ (void)counterIdx; (void)enable; return SCE_OK; }

static inline int scePerfArmPmcResetCounters(void) { return SCE_OK; }

static inline int scePerfArmPmcConfigureCounter(ScePerfArmPmcCounter counterIdx,
    uint32_t config)
{ (void)counterIdx; (void)config; return SCE_OK; }

static inline uint64_t scePerfGetTimebaseValue(void) { return 0; }
static inline uint32_t scePerfGetTimebaseFrequency(void) { return 1000000; }

#ifdef __cplusplus
}
#endif
