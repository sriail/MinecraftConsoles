# Compile Instructions — PS Vita Edition

This repository contains the **PS Vita** edition of Minecraft: Console Edition (LCE).

## Prerequisites

- [VitaSDK](https://vitasdk.org/) toolchain installed and on your `PATH`
- CMake 3.24 or newer
- A PS Vita development unit or a Vita3K emulator build for testing

## CMake (VitaSDK)

Configure with the VitaSDK toolchain:

```sh
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake
```

Build Debug:

```sh
cmake --build build --config Debug --target MinecraftClient
```

Build Release:

```sh
cmake --build build --config Release --target MinecraftClient
```

## Notes

- Building requires the PSP2 (VitaSDK) toolchain. The PS Vita uses an ARM Cortex-A9 CPU.
- PS Vita system libraries (`SceGxm`, `SceNet`, etc.) must be available through the VitaSDK sysroot.
- Media assets are located in `Minecraft.Client/PSVitaMedia/`.
- PS Vita system metadata (trophy data, livearea, param.sfo) is in `Minecraft.Client/sce_sys/`.
- Trophy data is in `Minecraft.Client/TROPDIR/`.

---

## WebAssembly (Emscripten) Build Guide

The PS Vita port can be compiled to WebAssembly and run in any modern browser using the
[Emscripten](https://emscripten.org/) toolchain. The WASM shim layer (under
`Minecraft.Client/PSVita/WASM/`) intercepts all proprietary Vita SDK calls and maps them
to standard browser APIs (WebGL 2, Web Audio, Gamepad API, WebSockets, IndexedDB).

### Prerequisites

| Tool | Minimum Version | Install |
|---|---|---|
| [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html) | 3.1.50+ | See step 1 below |
| CMake | 3.24+ | [cmake.org](https://cmake.org/download/) |
| Python 3 | 3.8+ | Required by emsdk |
| A modern browser | Chrome 92+ / Firefox 90+ | Must support `SharedArrayBuffer` (default pthreads build) |
| A local HTTP server | any | Must set COOP/COEP headers for pthreads build |

### Step 1 — Install and activate Emscripten

```sh
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate the latest SDK
./emsdk install latest
./emsdk activate latest

# Load the environment variables (repeat in every new shell session)
source ./emsdk_env.sh          # Linux / macOS
# On Windows (PowerShell): .\emsdk_env.ps1
# On Windows (cmd):        emsdk_env.bat
```

Verify the installation:

```sh
emcc --version
# Expected: emcc (Emscripten gcc/clang-like replacement + linker ...) 3.x.y
```

### Step 2 — Clone the repository

```sh
git clone https://github.com/sriail/MinecraftConsoles.git
cd MinecraftConsoles
```

### Step 3 — Configure with CMake + Emscripten

```sh
emcmake cmake -S . -B build-wasm \
    -DCMAKE_BUILD_TYPE=Release
```

`emcmake` automatically injects the Emscripten toolchain file and sets the
`EMSCRIPTEN` variable, which causes `CMakeLists.txt` to include
`cmake/vita_wasm.cmake` and apply the shim layer to `MinecraftClient`.

#### Optional CMake flags

| Flag | Default | Effect |
|---|---|---|
| `-DVITA_WASM_PTHREADS=OFF` | ON | Disable pthreads. Single-threaded build; no `SharedArrayBuffer`/COOP/COEP required. Slower. |
| `-DVITA_WASM_ASYNCIFY=OFF` | ON | Disable Asyncify. Faster compile but sync-blocking shims (audio output, vsync, network connect) will not block correctly. |
| `-DVITA_WASM_ES_MODULE=ON` | OFF | Emit an ES module (`import`) wrapper instead of a classic `<script>` tag. |

### Step 4 — Build

```sh
emmake cmake --build build-wasm --target MinecraftClient
```

The build output goes to `build-wasm/`:

```
build-wasm/
├── MinecraftClient.wasm    ← compiled game binary
├── MinecraftClient.js      ← Emscripten JS loader / glue
└── MinecraftClient.html    ← minimal test shell (auto-generated)
```

### Step 5 — Serve and run in a browser

WebAssembly with `SharedArrayBuffer` (required by the default pthreads build) needs
specific HTTP response headers. A plain `file://` URL will **not** work.

#### Option A — Python wrapper (recommended for local testing)

```sh
pip install simple-coop-server
simple-coop-server build-wasm --port 8080
```

Open: <http://localhost:8080/MinecraftClient.html>

#### Option B — Node.js / npx

```sh
npx serve build-wasm \
  --header "Cross-Origin-Opener-Policy: same-origin" \
  --header "Cross-Origin-Embedder-Policy: require-corp" \
  --listen tcp:8080
```

Open: <http://localhost:8080/MinecraftClient.html>

#### Option C — Single-threaded build (no special headers needed)

If you cannot configure the server headers, build without pthreads:

```sh
emcmake cmake -S . -B build-wasm-st \
    -DCMAKE_BUILD_TYPE=Release \
    -DVITA_WASM_PTHREADS=OFF

emmake cmake --build build-wasm-st --target MinecraftClient

# Serve with any plain static server
python3 -m http.server 8080 --directory build-wasm-st
```

> **Note:** The single-threaded build serialises all SDK subsystems onto the main
> browser thread. Audio, networking, and rendering will work, but performance will
> be lower than the pthreads build.

### Pre-build: mount the save-data filesystem

The game reads and writes save data via the `savedata0:` Vita path, which is mapped
to `/savedata/` in the Emscripten virtual filesystem backed by [IndexedDB (IDBFS)](https://emscripten.org/docs/api_reference/Filesystem-API.html#filesystem-api-idbfs).

Add the following initialisation block to your host HTML page **before** the
Emscripten module is loaded:

```html
<script>
var Module = {
  onRuntimeInitialized: function() {
    // Create and mount the IDBFS directories
    FS.mkdir('/savedata');
    FS.mkdir('/ux0');
    FS.mount(IDBFS, {}, '/savedata');
    FS.mount(IDBFS, {}, '/ux0');

    // Sync from IndexedDB into the in-memory FS, then start the game
    FS.syncfs(true, function(err) {
      if (err) console.error('FS sync failed:', err);
      callMain([]);
    });
  },

  // Flush writes back to IndexedDB before the page unloads
  onExit: function() {
    FS.syncfs(false, function(err) {
      if (err) console.error('FS sync (flush) failed:', err);
    });
  }
};
</script>
<script src="MinecraftClient.js"></script>
```

### Shim layer overview

The WASM build uses the shim layer under `Minecraft.Client/PSVita/WASM/`:

| Vita SDK module | Shim file | Browser API used |
|---|---|---|
| `SceKernel` (threads, mutexes, semaphores, event flags) | `shim_SceKernel.cpp` | Emscripten pthreads (`pthread_*`, `sem_t`) |
| `SceDisplay` (960×544 framebuffer, vsync) | `shim_SceDisplay.{cpp,js}` | HTML `<canvas>` Canvas 2D, `emscripten_sleep` |
| `SceCtrl` (dual-analog + 16 digital buttons) | `shim_SceCtrl.{cpp,js}` | W3C Gamepad API (Standard Mapping) |
| `SceAudio` (PCM 48 kHz/16-bit stereo) | `shim_SceAudio.{cpp,js}` | Web Audio API (`ScriptProcessorNode`) |
| `SceNet` BSD sockets (TCP only) | `shim_SceNet.{cpp,js}` | `WebSocket`; UDP → `SCE_NET_ERROR_ENOTSUP` |
| `SceSysmodule` (module load/unload) | `shim_SceSysmodule.cpp` | Stub; returns `SCE_OK` + `[VITA_SHIM_STUB]` log |
| `SceIoFileMgr` (file I/O, directories) | `shim_SceIoFileMgr.cpp` | Emscripten POSIX FS (IDBFS / MEMFS) |
| `SceGxm` (GPU render API, textures, draws) | `shim_SceGxm.{cpp,js}` | WebGL 2.0 (fallback WebGL 1.0) |

Unimplemented calls log `[VITA_SHIM_STUB]` to the browser console and return
`SCE_OK` so the game engine does not enter fatal error paths.

### Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| Black screen / no canvas | WebGL context creation failed | Check browser console; try a browser with WebGL 2 support |
| `SharedArrayBuffer is not defined` | Missing COOP/COEP headers | Use `simple-coop-server` or the single-threaded (`VITA_WASM_PTHREADS=OFF`) build |
| No controller input | Gamepad API requires a user gesture first | Click/press any key on the page before the game starts |
| No audio | Web Audio requires a user gesture to resume | Click on the canvas before audio plays |
| Save data lost on refresh | IDBFS not flushed on exit | Ensure `FS.syncfs(false, ...)` is called in `onExit` (see pre-build section above) |
| `emcmake` not found | Emscripten environment not loaded | Run `source /path/to/emsdk/emsdk_env.sh` first |
