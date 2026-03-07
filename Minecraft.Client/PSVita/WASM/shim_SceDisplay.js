/**
 * shim_SceDisplay.js
 * Emscripten JS-library glue for the SceDisplay shim.
 *
 * Exposes vita_display_blit() which is called from shim_SceDisplay.cpp
 * to transfer the Vita framebuffer (ARGB packed) to a Canvas 2D context.
 *
 * Usage:  emcc ... --js-library shim_SceDisplay.js
 */

mergeInto(LibraryManager.library, {
    vita_display_blit__deps: [],
    vita_display_blit: function(pixelPtr, width, height, pitch) {
        var canvas = document.getElementById('canvas');
        if (!canvas) return;

        /* Resize canvas to match the framebuffer if necessary */
        if (canvas.width !== width || canvas.height !== height) {
            canvas.width  = width;
            canvas.height = height;
        }

        var ctx = canvas.getContext('2d');
        if (!ctx) return;

        /* The Vita stores pixels as ARGB (0xAARRGGBB little-endian = BGRA in
         * memory byte order). Canvas ImageData expects RGBA. We must swizzle. */
        var src = Module.HEAPU8;
        var imageData = ctx.createImageData(width, height);
        var dst = imageData.data;

        for (var y = 0; y < height; y++) {
            var srcRow = pixelPtr + y * pitch * 4;
            var dstRow = y * width * 4;
            for (var x = 0; x < width; x++) {
                var b = src[srcRow + x * 4 + 0]; /* B */
                var g = src[srcRow + x * 4 + 1]; /* G */
                var r = src[srcRow + x * 4 + 2]; /* R */
                var a = src[srcRow + x * 4 + 3]; /* A */
                dst[dstRow + x * 4 + 0] = r;
                dst[dstRow + x * 4 + 1] = g;
                dst[dstRow + x * 4 + 2] = b;
                dst[dstRow + x * 4 + 3] = a;
            }
        }

        ctx.putImageData(imageData, 0, 0);
    }
});
