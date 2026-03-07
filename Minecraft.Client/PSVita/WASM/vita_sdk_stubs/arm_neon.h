#pragma once
/* arm_neon.h stub for WASM builds - NEON SIMD intrinsics are not available.
   Any code using these will need to be compiled with scalar fallback paths. */
#include <stdint.h>

/* Minimal NEON type stubs so code compiles - these are placeholders only */
typedef float float32_t;
typedef struct { float val[4]; } float32x4_t;
typedef struct { float val[2]; } float32x2_t;
typedef struct { int32_t val[4]; } int32x4_t;
typedef struct { int16_t val[8]; } int16x8_t;
typedef struct { int8_t val[16]; } int8x16_t;
typedef struct { uint8_t val[16]; } uint8x16_t;
typedef struct { uint16_t val[8]; } uint16x8_t;
typedef struct { uint32_t val[4]; } uint32x4_t;
