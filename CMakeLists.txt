cmake_minimum_required(VERSION 3.24)

project(MinecraftConsoles LANGUAGES C CXX)

# PS Vita builds require the PSP2 (VitaSDK) toolchain.
# Pass -DCMAKE_TOOLCHAIN_FILE=/path/to/vita.toolchain.cmake when configuring.
# See: https://github.com/vitasdk/vita-toolchain
#
# Emscripten/WASM builds use the Emscripten CMake toolchain.
# Pass -DCMAKE_TOOLCHAIN_FILE=/path/to/Emscripten.cmake when configuring.
# Example: emcmake cmake -S . -B build-wasm

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

# -----------------------------------------------------------------------
# Emscripten/WASM build configuration
# -----------------------------------------------------------------------
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  set(VITA_WASM_OVERRIDE_HEADER
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/WASM/wasm_platform_override.h")

  set(VITA_WASM_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/WASM/vita_sdk_stubs"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/PSVitaExtras"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/Network"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client"
  )

  set(VITA_WASM_DEFINES
    _PSVita
    _PSVita=1
    __PSVITA__=1
    __psp2__=1
    _CRT_NON_CONFORMING_SWPRINTFS
    _CRT_SECURE_NO_WARNINGS
    _DEBUG_MENUS_ENABLED
    _LARGE_WORLDS
    _LIB
  )
endif()

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

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  target_include_directories(MinecraftWorld BEFORE PRIVATE ${VITA_WASM_INCLUDE_DIRS})
  target_compile_definitions(MinecraftWorld PRIVATE ${VITA_WASM_DEFINES})
  target_compile_options(MinecraftWorld PRIVATE
    "-include${VITA_WASM_OVERRIDE_HEADER}"
  )
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

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  target_include_directories(MinecraftClient BEFORE PRIVATE ${VITA_WASM_INCLUDE_DIRS})
  target_compile_definitions(MinecraftClient PRIVATE ${VITA_WASM_DEFINES})
  target_compile_options(MinecraftClient PRIVATE
    "-include${VITA_WASM_OVERRIDE_HEADER}"
  )
endif()

set_target_properties(MinecraftClient PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:MinecraftClient>"
)

target_link_libraries(MinecraftClient PRIVATE
  MinecraftWorld
  $<$<AND:$<CONFIG:Debug>,$<NOT:$<STREQUAL:${CMAKE_SYSTEM_NAME},Emscripten>>>:
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Input_d.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Storage_d.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Render_PSVita_d.lib"
  >
  $<$<AND:$<NOT:$<CONFIG:Debug>>,$<NOT:$<STREQUAL:${CMAKE_SYSTEM_NAME},Emscripten>>>:
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Input.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Storage.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/libs/4J_Render_PSVita.lib"
  >
)

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
