# vita_wasm.cmake
# CMake integration for the PS Vita → WASM shim layer.
#
# Usage (from top-level CMakeLists.txt or a platform-specific toolchain):
#
#   include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/vita_wasm.cmake")
#   vita_wasm_link(MinecraftClient)
#
# Requirements:
#   - Emscripten toolchain: pass
#       -DCMAKE_TOOLCHAIN_FILE=/path/to/Emscripten.cmake
#     (or use emcmake / emcmake wrappers)
#   - emcc must be on PATH
#
# Generated artefact: MinecraftClient.wasm + MinecraftClient.js (ES module or
# classic, depending on VITA_WASM_ES_MODULE option below).

# -----------------------------------------------------------------------
# Options
# -----------------------------------------------------------------------
option(VITA_WASM_ES_MODULE  "Emit an ES-module WASM wrapper"  OFF)
option(VITA_WASM_PTHREADS   "Enable WASM SharedArrayBuffer pthreads" ON)
option(VITA_WASM_ASYNCIFY   "Enable Asyncify for sync-blocking shims" ON)

# -----------------------------------------------------------------------
# Shim source files
# -----------------------------------------------------------------------
set(VITA_WASM_SHIM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/WASM")

set(VITA_WASM_SHIM_SOURCES
    "${VITA_WASM_SHIM_DIR}/shim_SceKernel.cpp"
    "${VITA_WASM_SHIM_DIR}/shim_SceDisplay.cpp"
    "${VITA_WASM_SHIM_DIR}/shim_SceCtrl.cpp"
    "${VITA_WASM_SHIM_DIR}/shim_SceAudio.cpp"
    "${VITA_WASM_SHIM_DIR}/shim_SceNet.cpp"
    "${VITA_WASM_SHIM_DIR}/shim_SceSysmodule.cpp"
    "${VITA_WASM_SHIM_DIR}/shim_SceIoFileMgr.cpp"
    "${VITA_WASM_SHIM_DIR}/shim_SceGxm.cpp"
)

set(VITA_WASM_JS_LIBS
    "--js-library ${VITA_WASM_SHIM_DIR}/shim_SceDisplay.js"
    "--js-library ${VITA_WASM_SHIM_DIR}/shim_SceCtrl.js"
    "--js-library ${VITA_WASM_SHIM_DIR}/shim_SceAudio.js"
    "--js-library ${VITA_WASM_SHIM_DIR}/shim_SceNet.js"
    "--js-library ${VITA_WASM_SHIM_DIR}/shim_SceGxm.js"
)

# -----------------------------------------------------------------------
# Function: vita_wasm_link(<target>)
#   Adds shim sources to <target> and configures the required Emscripten
#   link options.
# -----------------------------------------------------------------------
function(vita_wasm_link target)
    # Add shim source files to the target
    target_sources(${target} PRIVATE ${VITA_WASM_SHIM_SOURCES})

    # Include directory for shim headers
    target_include_directories(${target} PRIVATE "${VITA_WASM_SHIM_DIR}")

    # ---- Compile flags ----
    target_compile_options(${target} PRIVATE
        -DVITA_WASM_BUILD=1
        -D__EMSCRIPTEN__
        # Emscripten defines these automatically; mirror them for IDE indexing
    )

    # ---- Emscripten link flags ----
    set(_emcc_flags "")

    # WebGL 2.0
    list(APPEND _emcc_flags
        "SHELL:-s USE_WEBGL2=1"
        "SHELL:-s FULL_ES3=1"
    )

    # pthreads (SharedArrayBuffer)
    if(VITA_WASM_PTHREADS)
        list(APPEND _emcc_flags
            "SHELL:-s USE_PTHREADS=1"
            "SHELL:-s PTHREAD_POOL_SIZE=16"
            "SHELL:-s SHARED_MEMORY=1"
        )
    endif()

    # Asyncify (for sync-blocking shims: audio, display vsync, network recv)
    if(VITA_WASM_ASYNCIFY)
        list(APPEND _emcc_flags
            "SHELL:-s ASYNCIFY=1"
            "SHELL:-s ASYNCIFY_IMPORTS=[\"emscripten_sleep\",\"vita_net_ws_connect\"]"
        )
    endif()

    # IndexedDB filesystem (save data)
    list(APPEND _emcc_flags
        "SHELL:-s FORCE_FILESYSTEM=1"
        "SHELL:-s EXTRA_EXPORTED_RUNTIME_METHODS=[\"FS\",\"callMain\"]"
    )

    # Memory: 256 MB initial, 512 MB maximum (Vita has 512 MB LPDDR2)
    list(APPEND _emcc_flags
        "SHELL:-s INITIAL_MEMORY=268435456"
        "SHELL:-s MAXIMUM_MEMORY=536870912"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    )

    # Exception handling (game uses C++ exceptions)
    list(APPEND _emcc_flags
        "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
    )

    # ES module output
    if(VITA_WASM_ES_MODULE)
        list(APPEND _emcc_flags "SHELL:-s EXPORT_ES6=1")
    endif()

    # JS library glue files
    foreach(_lib ${VITA_WASM_JS_LIBS})
        list(APPEND _emcc_flags "SHELL:${_lib}")
    endforeach()

    # Export symbols needed by JS libs
    list(APPEND _emcc_flags
        "SHELL:-s EXPORTED_FUNCTIONS=[\"_main\",\"_vita_ctrl_update\",\"_vita_net_ws_push_recv\"]"
    )

    target_link_options(${target} PRIVATE ${_emcc_flags})

    message(STATUS "[vita_wasm] Configured WASM shim layer for target: ${target}")
endfunction()
