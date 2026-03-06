#ifndef SHIM_SCECTRL_H
#define SHIM_SCECTRL_H

/*
 * shim_SceCtrl.h
 * PS Vita SceCtrl → Web Gamepad API shim
 *
 * Covers: digital button polling, analog stick reading, touch-pad stubs.
 *
 * Standard Gamepad Mapping (W3C) button indices used for translation:
 *   0  = Cross (A)       1  = Circle (B)      2  = Square (X)   3  = Triangle (Y)
 *   4  = L1              5  = R1              6  = L2           7  = R2
 *   8  = Select/Share    9  = Start/Options   10 = L3           11 = R3
 *   12 = D-Up            13 = D-Down          14 = D-Left       15 = D-Right
 *   16 = Home/PS
 *
 * ABI reference: vitasdk <ctrl.h>
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Return codes ---------- */
#ifndef SCE_OK
#define SCE_OK 0
#endif
#define SCE_CTRL_ERROR_INVALID_ARG  ((int)0x80340001)

/* ---------- Button bitmask definitions (match vitasdk) ---------- */
#define SCE_CTRL_SELECT     0x00000001
#define SCE_CTRL_L3         0x00000002
#define SCE_CTRL_R3         0x00000004
#define SCE_CTRL_START      0x00000008
#define SCE_CTRL_UP         0x00000010
#define SCE_CTRL_RIGHT      0x00000020
#define SCE_CTRL_DOWN       0x00000040
#define SCE_CTRL_LEFT       0x00000080
#define SCE_CTRL_LTRIGGER   0x00000100
#define SCE_CTRL_RTRIGGER   0x00000200
#define SCE_CTRL_L1         SCE_CTRL_LTRIGGER
#define SCE_CTRL_R1         SCE_CTRL_RTRIGGER
#define SCE_CTRL_L2         0x00000800
#define SCE_CTRL_R2         0x00001000
#define SCE_CTRL_TRIANGLE   0x00010000
#define SCE_CTRL_CIRCLE     0x00020000
#define SCE_CTRL_CROSS      0x00040000
#define SCE_CTRL_SQUARE     0x00080000
#define SCE_CTRL_ANY        0xFFFFFFFF

/* ---------- Sampling mode ---------- */
#define SCE_CTRL_MODE_DIGITAL   0
#define SCE_CTRL_MODE_ANALOG    1
#define SCE_CTRL_MODE_ANALOG_WIDE 2

/* ---------- Data struct ---------- */
typedef struct SceCtrlData {
    uint64_t timeStamp;
    uint32_t buttons;
    uint8_t  lx;   /* Left  stick X: 0-255, center=128 */
    uint8_t  ly;   /* Left  stick Y: 0-255, center=128 */
    uint8_t  rx;   /* Right stick X */
    uint8_t  ry;   /* Right stick Y */
    uint8_t  lt;   /* L2 pressure 0-255 */
    uint8_t  rt;   /* R2 pressure 0-255 */
    uint8_t  reserved[10];
} SceCtrlData;

/* ---------- API ---------- */
int sceCtrlSetSamplingMode(int mode);
int sceCtrlReadBufferPositive(int port, SceCtrlData *pData, int count);
int sceCtrlReadBufferNegative(int port, SceCtrlData *pData, int count);
int sceCtrlPeekBufferPositive(int port, SceCtrlData *pData, int count);

#ifdef __cplusplus
}
#endif

#endif /* SHIM_SCECTRL_H */
