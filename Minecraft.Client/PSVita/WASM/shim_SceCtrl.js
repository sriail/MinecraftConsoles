/**
 * shim_SceCtrl.js
 * Emscripten JS-library glue for the SceCtrl shim.
 *
 * Polls the W3C Gamepad API and maps the Standard Mapping to Vita button bits.
 * vita_ctrl_poll() is called from C every time sceCtrlReadBufferPositive runs.
 *
 * Standard Mapping → Vita SceCtrl button bit mapping:
 *   GP btn 0  (Cross/A)     → SCE_CTRL_CROSS     (0x4000)
 *   GP btn 1  (Circle/B)    → SCE_CTRL_CIRCLE    (0x2000)
 *   GP btn 2  (Square/X)    → SCE_CTRL_SQUARE    (0x8000)
 *   GP btn 3  (Triangle/Y)  → SCE_CTRL_TRIANGLE  (0x1000)
 *   GP btn 4  (L1)          → SCE_CTRL_LTRIGGER  (0x0100)
 *   GP btn 5  (R1)          → SCE_CTRL_RTRIGGER  (0x0200)
 *   GP btn 8  (Select)      → SCE_CTRL_SELECT    (0x0001)
 *   GP btn 9  (Start)       → SCE_CTRL_START     (0x0008)
 *   GP btn 10 (L3)          → SCE_CTRL_L3        (0x0002)
 *   GP btn 11 (R3)          → SCE_CTRL_R3        (0x0004)
 *   GP btn 12 (D-Up)        → SCE_CTRL_UP        (0x0010)
 *   GP btn 13 (D-Down)      → SCE_CTRL_DOWN      (0x0040)
 *   GP btn 14 (D-Left)      → SCE_CTRL_LEFT      (0x0080)
 *   GP btn 15 (D-Right)     → SCE_CTRL_RIGHT     (0x0020)
 *   Axis 2 (L2 analog)      → SCE_CTRL_L2        (0x0800) when > 0.1
 *   Axis 5 (R2 analog)      → SCE_CTRL_R2        (0x1000) when > 0.1
 *
 * Usage:  emcc ... --js-library shim_SceCtrl.js
 */

mergeInto(LibraryManager.library, {
    vita_ctrl_poll__deps: ['vita_ctrl_update'],
    vita_ctrl_poll: function() {
        var gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
        var gp = null;
        for (var i = 0; i < gamepads.length; i++) {
            if (gamepads[i] && gamepads[i].connected) { gp = gamepads[i]; break; }
        }

        var buttons = 0;
        var lx = 0.0, ly = 0.0, rx = 0.0, ry = 0.0, lt = 0.0, rt = 0.0;

        if (gp) {
            var b = gp.buttons;
            var a = gp.axes;

            var pressed = function(idx) {
                return (b[idx] && (typeof b[idx] === 'object' ? b[idx].pressed : b[idx] > 0.5));
            };

            /* Digital face buttons */
            if (pressed(0))  buttons |= 0x40000; /* Cross    */
            if (pressed(1))  buttons |= 0x20000; /* Circle   */
            if (pressed(2))  buttons |= 0x80000; /* Square   */
            if (pressed(3))  buttons |= 0x10000; /* Triangle */
            if (pressed(4))  buttons |= 0x0100;  /* L1       */
            if (pressed(5))  buttons |= 0x0200;  /* R1       */
            if (pressed(8))  buttons |= 0x0001;  /* Select   */
            if (pressed(9))  buttons |= 0x0008;  /* Start    */
            if (pressed(10)) buttons |= 0x0002;  /* L3       */
            if (pressed(11)) buttons |= 0x0004;  /* R3       */
            if (pressed(12)) buttons |= 0x0010;  /* D-Up     */
            if (pressed(13)) buttons |= 0x0040;  /* D-Down   */
            if (pressed(14)) buttons |= 0x0080;  /* D-Left   */
            if (pressed(15)) buttons |= 0x0020;  /* D-Right  */

            /* Analog sticks */
            lx = a[0] || 0.0;
            ly = a[1] || 0.0;
            rx = a[2] || 0.0;
            ry = a[3] || 0.0;

            /* L2 / R2 triggers (axes 4 and 5, range -1..1, rest at -1) */
            lt = a[4] !== undefined ? (a[4] + 1.0) / 2.0 : 0.0;
            rt = a[5] !== undefined ? (a[5] + 1.0) / 2.0 : 0.0;
            if (lt > 0.1) buttons |= 0x0800; /* L2 */
            if (rt > 0.1) buttons |= 0x1000; /* R2 */
        }

        /* Call C function to store state */
        Module._vita_ctrl_update(buttons, lx, ly, rx, ry, lt, rt);
    }
});
