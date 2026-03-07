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

## WebAssembly Build (Emscripten)

The PS Vita source can be compiled to WebAssembly using Emscripten.  
All Sony SDK calls are redirected to open, portable stubs inside
`Minecraft.Client/PSVita/WASM/` — **no real PS Vita SDK is required**.

### Prerequisites

| Tool | Min version | Notes |
|------|-------------|-------|
| [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html) | 3.1.50 | Install with `./emsdk install latest && ./emsdk activate latest` |
| CMake | 3.24 | [cmake.org/download](https://cmake.org/download/) |
| Python 3 | any | Required for `emcmake` and the dev HTTP server |
| Ninja *(optional)* | any | Faster parallel builds than GNU Make |

### 1 — Install & activate Emscripten

```bash
# Clone emsdk once (skip if you already have it)
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate the latest SDK
./emsdk install latest
./emsdk activate latest

# Activate environment variables (run this in every new shell)
source ./emsdk_env.sh          # Linux / macOS
# .\emsdk_env.bat               # Windows (cmd)
# .\emsdk_env.ps1               # Windows (PowerShell)
```

Verify the installation:

```bash
emcc --version
# emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 3.x.x
```

### 2 — Configure the WASM build

```bash
# From the repository root
emcmake cmake -S . -B build-wasm \
  -DCMAKE_BUILD_TYPE=Release
```

**Optional CMake variables**

| Variable | Default | Description |
|----------|---------|-------------|
| `WASM_INITIAL_MEMORY_MB` | `256` | Initial WebAssembly heap size in MB. Memory can still grow beyond this (ALLOW_MEMORY_GROWTH is enabled). |
| `WASM_STACK_SIZE_MB` | `8` | WebAssembly stack size in MB. |

Example with custom memory:

```bash
emcmake cmake -S . -B build-wasm \
  -DCMAKE_BUILD_TYPE=Release \
  -DWASM_INITIAL_MEMORY_MB=512 \
  -DWASM_STACK_SIZE_MB=16
```

### 3 — Build

```bash
# Uses all available CPU cores
cmake --build build-wasm --target MinecraftClient -j$(nproc)

# Or with Ninja (faster):
# emcmake cmake -S . -B build-wasm -GNinja -DCMAKE_BUILD_TYPE=Release
# cmake --build build-wasm
```

Emscripten produces three output files in `build-wasm/`:

| File | Purpose |
|------|---------|
| `MinecraftClient.html` | Entry point — open this in a browser |
| `MinecraftClient.js` | Emscripten JS glue code |
| `MinecraftClient.wasm` | WebAssembly binary |

### 4 — Serve and run in a browser

Browsers block `file://` access to WASM modules; you must serve the files over HTTP.

**Python 3 (simplest)**

```bash
cd build-wasm
python3 -m http.server 8080
```

Open **http://localhost:8080/MinecraftClient.html** in Chrome, Firefox, or Edge.

**Node.js / http-server**

```bash
npx http-server build-wasm -p 8080 --cors
```

**VS Code Live Server extension**

Right-click `MinecraftClient.html` inside the `build-wasm/` folder and choose
*"Open with Live Server"*.

### 5 — Loading game assets

The game reads assets (textures, sounds, levels) relative to its working directory.
On WASM the virtual filesystem is empty by default. Two options:

**Option A — Preload at build time (recommended)**

Add `--preload-file` to the CMake `target_link_options` block in `CMakeLists.txt`:

```cmake
# Inside the if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten") block:
target_link_options(MinecraftClient PRIVATE
  ...
  "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/@/PSVita/"
)
```

This embeds the assets into a `.data` file that is downloaded alongside the WASM.

**Option B — Copy assets after build**

```bash
cp -r Minecraft.Client/PSVita/PSVitaMedia build-wasm/PSVitaMedia
cp -r Minecraft.Client/PSVita/sce_sys     build-wasm/sce_sys
```

Then re-serve from `build-wasm/`.

### Architecture overview

```
CMakeLists.txt
└─ if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
   ├─ vita_sdk_stubs/   ← intercepts <kernel.h>, <gxm.h>, <np.h>, etc.
   │   31 stub headers with inline no-op implementations
   ├─ wasm_platform_override.h  ← force-included before every TU
   │   defines __PSVITA__, __psp2__, SCE_DBG_LOG, display macros
   └─ platform_wasm.cpp  ← runtime stubs (TLS, critical sections,
                           VirtualAlloc, GetTickCount, Sleep, …)
```

### Troubleshooting

| Error | Likely cause | Fix |
|-------|-------------|-----|
| `SharedArrayBuffer is not defined` | Browser requires cross-origin isolation headers | Serve with `Cross-Origin-Opener-Policy: same-origin` and `Cross-Origin-Embedder-Policy: require-corp` headers |
| `Out of memory` | Default 256 MB heap too small | Rebuild with `-DWASM_INITIAL_MEMORY_MB=512` |
| Blank / black screen | Assets not found | Follow step 5 (load game assets) above |
| `emcmake: command not found` | emsdk environment not activated | Run `source /path/to/emsdk/emsdk_env.sh` |
| WASM file missing after build | Wrong target name | Use `--target MinecraftClient` explicitly |
