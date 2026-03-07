cmake_minimum_required(VERSION 3.24)

project(MinecraftConsoles LANGUAGES C CXX)

# PS Vita builds require the PSP2 (VitaSDK) toolchain.
# Pass -DCMAKE_TOOLCHAIN_FILE=/path/to/vita.toolchain.cmake when configuring.
# See: https://github.com/vitasdk/vita-toolchain
#
# Emscripten / WASM builds:
# emcmake cmake -B build-wasm -DCMAKE_BUILD_TYPE=Release
# cmake --build build-wasm

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

function(configure_msvc_target target)
  target_compile_options(${target} PRIVATE
    $<$<AND:$<NOT:$<CONFIG:Release>>,$<COMPILE_LANGUAGE:C,CXX>>:/W3>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C,CXX>>:/W0>
    $<$<COMPILE_LANGUAGE:C,CXX>:/MP>
    $<$<COMPILE_LANGUAGE:C,CXX>:/FS>
    $<$<COMPILE_LANGUAGE:CXX>:/EHsc>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C,CXX>>:/GL /O2 /Oi /GT /GF>
  )
endfunction()

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/WorldSources.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ClientSources.cmake")

list(TRANSFORM MINECRAFT_WORLD_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.World/")
list(TRANSFORM MINECRAFT_CLIENT_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/")

# ── Emscripten / WASM build configuration ──────────────────────────────────
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  # Directories containing PS Vita SDK stub headers
  set(VITA_WASM_STUB_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/WASM/vita_sdk_stubs"
  )

  # Emscripten heap and stack sizes (configurable via -DWASM_INITIAL_MEMORY_MB=256)
  set(WASM_INITIAL_MEMORY_MB 256 CACHE STRING "Emscripten initial heap size in MB")
  set(WASM_STACK_SIZE_MB      8  CACHE STRING "Emscripten stack size in MB")
  math(EXPR WASM_INITIAL_MEMORY "${WASM_INITIAL_MEMORY_MB} * 1024 * 1024")
  math(EXPR WASM_STACK_SIZE     "${WASM_STACK_SIZE_MB}     * 1024 * 1024")

  # All PSVita include paths needed for Emscripten
  set(VITA_WASM_INCLUDE_DIRS
    "${VITA_WASM_STUB_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/PSVitaExtras"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/Network"
  )

  # Force-include the platform override header before every translation unit
  set(VITA_WASM_FORCE_INCLUDE
    "-include${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/WASM/wasm_platform_override.h"
  )

  # platform_wasm.cpp provides runtime stubs for WASM builds
  list(APPEND MINECRAFT_CLIENT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/WASM/platform_wasm.cpp"
  )
endif()
# ────────────────────────────────────────────────────────────────────────────

add_library(MinecraftWorld STATIC ${MINECRAFT_WORLD_SOURCES})
target_include_directories(MinecraftWorld PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.World"
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.World/x64headers"
)
target_compile_definitions(MinecraftWorld PRIVATE
  $<$<CONFIG:Debug>:_LARGE_WORLDS;_DEBUG_MENUS_ENABLED;_DEBUG;_LIB;_CRT_NON_CONFORMING_SWPRINTFS;_CRT_SECURE_NO_WARNINGS;_PSVita>
  $<$<NOT:$<CONFIG:Debug>>:_LARGE_WORLDS;_DEBUG_MENUS_ENABLED;_LIB;_CRT_NON_CONFORMING_SWPRINTFS;_CRT_SECURE_NO_WARNINGS;_PSVita>
)
if(MSVC)
  configure_msvc_target(MinecraftWorld)
endif()

# Apply Emscripten-specific settings to MinecraftWorld
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  target_include_directories(MinecraftWorld PRIVATE ${VITA_WASM_INCLUDE_DIRS})
  target_compile_definitions(MinecraftWorld PRIVATE
    __PSVITA__=1 __psp2__=1 _PSVita=1
  )
  target_compile_options(MinecraftWorld PRIVATE ${VITA_WASM_FORCE_INCLUDE})
endif()

add_executable(MinecraftClient ${MINECRAFT_CLIENT_SOURCES})
target_include_directories(MinecraftClient PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client"
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/Iggy/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/Sentient"
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.World/x64headers"
)
target_compile_definitions(MinecraftClient PRIVATE
  $<$<CONFIG:Debug>:_LARGE_WORLDS;_DEBUG_MENUS_ENABLED;_DEBUG;_CRT_NON_CONFORMING_SWPRINTFS;_CRT_SECURE_NO_WARNINGS;_PSVita>
  $<$<NOT:$<CONFIG:Debug>>:_LARGE_WORLDS;_DEBUG_MENUS_ENABLED;_CRT_NON_CONFORMING_SWPRINTFS;_CRT_SECURE_NO_WARNINGS;_PSVita>
)
if(MSVC)
  configure_msvc_target(MinecraftClient)
  target_link_options(MinecraftClient PRIVATE
    $<$<CONFIG:Release>:/LTCG /INCREMENTAL:NO>
  )
endif()

set_target_properties(MinecraftClient PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:MinecraftClient>"
)

# Apply Emscripten-specific settings to MinecraftClient
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  target_include_directories(MinecraftClient PRIVATE ${VITA_WASM_INCLUDE_DIRS})
  target_compile_definitions(MinecraftClient PRIVATE
    __PSVITA__=1 __psp2__=1 _PSVita=1
  )
  target_compile_options(MinecraftClient PRIVATE ${VITA_WASM_FORCE_INCLUDE})
  # Emscripten linker flags: generate HTML + JS + WASM, enable file system access
  target_link_options(MinecraftClient PRIVATE
    "-sALLOW_MEMORY_GROWTH=1"
    "-sINITIAL_MEMORY=${WASM_INITIAL_MEMORY}"
    "-sSTACK_SIZE=${WASM_STACK_SIZE}"
    "-sFORCE_FILESYSTEM=1"
    "-sEXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\",\"FS\"]"
    "--bind"
  )
  set_target_properties(MinecraftClient PROPERTIES
    SUFFIX ".html"
  )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  # WASM builds do not link .lib / .a stub libraries
  target_link_libraries(MinecraftClient PRIVATE MinecraftWorld)
else()
  target_link_libraries(MinecraftClient PRIVATE
    MinecraftWorld
    $<$<CONFIG:Debug>:
      "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Input_d.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Storage_d.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Render_PSVita_d.lib"
    >
    $<$<NOT:$<CONFIG:Debug>>:
      "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Input.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Storage.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Render_PSVita.lib"
    >
  )
endif()

if(CMAKE_HOST_WIN32)
  message(STATUS "Starting asset copy...")
  execute_process(
    COMMAND robocopy.exe
            "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client"
            "${CMAKE_CURRENT_BINARY_DIR}"
            /S /MT /R:0 /W:0 /NP
            /XF "*.cpp" "*.c" "*.h" "*.hpp" "*.asm"
                "*.xml" "*.lang" "*.vcxproj" "*.vcxproj.*" "*.sln"
                "*.docx" "*.xls"
                "*.bat" "*.cmd" "*.ps1" "*.py"
                "*Test*"
            /XD "Durango*" "Orbis*" "PS3*" "PS4*" "PS3_GAME" "PS4_GAME" "Xbox" "Windows64*"
  )
elseif(CMAKE_HOST_UNIX)
  message(STATUS "Starting asset copy...")
  execute_process(
    COMMAND rsync -av
            "--exclude=*.cpp" "--exclude=*.c" "--exclude=*.h" "--exclude=*.hpp" "--exclude=*.asm"
            "--exclude=*.xml" "--exclude=*.lang" "--exclude=*.vcxproj" "--exclude=*.vcxproj.*" "--exclude=*.sln"
            "--exclude=*.docx" "--exclude=*.xls"
            "--exclude=*.bat" "--exclude=*.cmd" "--exclude=*.ps1" "--exclude=*.py"
            "--exclude=*Test*"
            "--exclude=Durango*" "--exclude=Orbis*" "--exclude=PS3*" "--exclude=PS4*"
            "--exclude=PS3_GAME" "--exclude=PS4_GAME" "--exclude=Xbox" "--exclude=Windows64*"
            "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/" "${CMAKE_CURRENT_BINARY_DIR}/"
  )
else()
  message(FATAL_ERROR "Asset copying is only supported on Windows (Robocopy) and Unix systems (rsync).")
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT MinecraftClient)
