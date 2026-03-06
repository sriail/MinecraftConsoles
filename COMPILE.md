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
