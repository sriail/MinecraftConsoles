/**
 * shim_SceGxm.js
 * Emscripten JS-library glue for the SceGxm shim.
 *
 * Provides a thin WebGL2 wrapper that the C shim calls via EM_ASM.
 * Maintains a single global WebGL2 rendering context bound to the
 * #canvas element.
 *
 * Build requirements:
 *   -s USE_WEBGL2=1
 *   -s FULL_ES3=1
 *
 * Usage:  emcc ... --js-library shim_SceGxm.js
 */

mergeInto(LibraryManager.library, {

    /* ------------------------------------------------------------------ */
    vita_gxm_initialize__deps: [],
    vita_gxm_initialize: function() {
        var canvas = document.getElementById('canvas');
        if (!canvas) return 0;
        canvas.width  = 960;
        canvas.height = 544;
        var gl = canvas.getContext('webgl2', {
            alpha: false,
            depth: true,
            stencil: true,
            antialias: false,
            premultipliedAlpha: false,
            preserveDrawingBuffer: false
        });
        if (!gl) {
            /* Fallback to WebGL 1 */
            gl = canvas.getContext('webgl', {
                alpha: false, depth: true, stencil: true
            });
        }
        if (!gl) return 0;
        Module._vita_gl = gl;
        return 1;
    },

    /* ------------------------------------------------------------------ */
    vita_gxm_terminate__deps: [],
    vita_gxm_terminate: function() {
        Module._vita_gl = null;
    },

    /* ------------------------------------------------------------------ */
    vita_gxm_begin_scene__deps: [],
    vita_gxm_begin_scene: function(width, height) {
        var gl = Module._vita_gl;
        if (!gl) return;
        gl.bindFramebuffer(gl.FRAMEBUFFER, null);
        gl.viewport(0, 0, width, height);
        gl.clearColor(0, 0, 0, 1);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT |
                 gl.STENCIL_BUFFER_BIT);
    },

    /* ------------------------------------------------------------------ */
    vita_gxm_end_scene__deps: [],
    vita_gxm_end_scene: function() {
        /* No explicit swap needed for WebGL; the browser compositor handles it */
    },

    /* ------------------------------------------------------------------ */
    vita_gxm_finish__deps: [],
    vita_gxm_finish: function() {
        var gl = Module._vita_gl;
        if (gl) gl.finish();
    },

    /* ------------------------------------------------------------------ */
    vita_gxm_texture_create__deps: [],
    vita_gxm_texture_create: function(dataPtr, width, height,
                                       internalFmt, fmt, type) {
        var gl = Module._vita_gl;
        if (!gl) return 0;
        var tex = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, tex);
        if (dataPtr) {
            var pixels = Module.HEAPU8.subarray(
                dataPtr, dataPtr + width * height * 4);
            gl.texImage2D(gl.TEXTURE_2D, 0, internalFmt, width, height, 0,
                          fmt, type, pixels);
        } else {
            gl.texImage2D(gl.TEXTURE_2D, 0, internalFmt, width, height, 0,
                          fmt, type, null);
        }
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);

        /* Store in a JS-side map indexed by a simple integer handle */
        if (!Module._vita_gxm_textures) {
            Module._vita_gxm_textures = {};
            Module._vita_gxm_tex_next_id = 1;
        }
        var id = Module._vita_gxm_tex_next_id++;
        Module._vita_gxm_textures[id] = tex;
        return id;
    },

    /* ------------------------------------------------------------------ */
    vita_gxm_texture_set_filter__deps: [],
    vita_gxm_texture_set_filter: function(texId, glFilter, isMag) {
        var gl  = Module._vita_gl;
        if (!gl || !Module._vita_gxm_textures) return;
        var tex = Module._vita_gxm_textures[texId];
        if (!tex) return;
        gl.bindTexture(gl.TEXTURE_2D, tex);
        if (isMag) {
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, glFilter);
        } else {
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, glFilter);
        }
    },

    /* ------------------------------------------------------------------ */
    vita_gxm_draw__deps: [],
    vita_gxm_draw: function(glPrimitive, glIndexType, indexDataPtr,
                             indexCount) {
        var gl = Module._vita_gl;
        if (!gl) return;
        if (indexDataPtr) {
            /* Upload index data to a temporary index buffer */
            var stride = (glIndexType === 0x1403 /* UNSIGNED_SHORT */) ? 2 : 4;
            var bytes  = indexCount * stride;
            var buf    = gl.createBuffer();
            gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, buf);
            var view = (stride === 2)
                ? new Uint16Array(Module.HEAPU8.buffer, indexDataPtr, indexCount)
                : new Uint32Array(Module.HEAPU8.buffer, indexDataPtr, indexCount);
            gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, view, gl.STREAM_DRAW);
            gl.drawElements(glPrimitive, indexCount, glIndexType, 0);
            gl.deleteBuffer(buf);
        } else {
            gl.drawArrays(glPrimitive, 0, indexCount);
        }
    }
});
