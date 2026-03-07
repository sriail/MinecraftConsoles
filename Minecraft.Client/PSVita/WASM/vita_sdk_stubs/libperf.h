#pragma once
// Minimal PS Vita libperf.h stub for Emscripten/WASM builds.
#include <stdint.h>
#define SCE_PERF_ARM_PMU_CYCLE_COUNT 0xFF
#ifdef __cplusplus
extern "C" {
#endif
int  scePerfArmPmonOpen(int unit, int *context);
int  scePerfArmPmonClose(int context);
int  scePerfArmPmonReset(int context);
int  scePerfArmPmonStartCounting(int context);
int  scePerfArmPmonStopCounting(int context);
int  scePerfArmPmonGetCounterValue(int context, uint32_t counter, uint64_t *val);
int  scePerfArmPmonSetCounterDistribution(int context, uint32_t counter, uint32_t event);
int  scePerfArmPmonClearCount(int context, uint32_t counter);
#ifdef __cplusplus
}
#endif
