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

## Emscripten / WebAssembly (WASM) Build

The codebase can be compiled to WebAssembly using
[Emscripten](https://emscripten.org/). The `Minecraft.Client/PSVita/WASM/`
directory contains the shim layer that replaces proprietary PS Vita SDK headers
with portable stubs.

### Prerequisites

- Emscripten SDK installed and activated (see https://emscripten.org/docs/getting_started/downloads.html)
- CMake 3.24 or newer

### Configure and Build

```sh
emcmake cmake -S . -B build-wasm
cmake --build build-wasm --target MinecraftClient
```

Alternatively with an explicit configuration type:

```sh
emcmake cmake -S . -B build-wasm -DCMAKE_BUILD_TYPE=Release
cmake --build build-wasm --config Release --target MinecraftClient
```

### How it Works

When the Emscripten toolchain is detected (`CMAKE_SYSTEM_NAME` equals
`"Emscripten"`), CMake automatically:

1. Prepends `Minecraft.Client/PSVita/WASM/vita_sdk_stubs/` to the include path
   so that `<kernel.h>`, `<np.h>`, `<touch.h>`, etc. resolve to the portable
   stub headers instead of the proprietary PS Vita SDK headers.
2. Force-includes `Minecraft.Client/PSVita/WASM/wasm_platform_override.h`
   before every translation unit to define `PSVITA_STUBBED`, `SCE_BREAK`, and
   `SCE_OK` for the WASM target.
3. Adds the `__PSVITA__=1`, `__psp2__=1`, and `_PSVita` preprocessor macros
   so that the PS Vita code path is selected throughout the codebase.
4. Skips linking against the proprietary `.lib` files that are only available
   with the real PS Vita SDK.

## Notes

- Building requires the PSP2 (VitaSDK) toolchain. The PS Vita uses an ARM Cortex-A9 CPU.
- PS Vita system libraries (`SceGxm`, `SceNet`, etc.) must be available through the VitaSDK sysroot.
- Media assets are located in `Minecraft.Client/PSVitaMedia/`.
- PS Vita system metadata (trophy data, livearea, param.sfo) is in `Minecraft.Client/sce_sys/`.
- Trophy data is in `Minecraft.Client/TROPDIR/`.
