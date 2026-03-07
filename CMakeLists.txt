cmake_minimum_required(VERSION 3.24)

project(MinecraftConsoles LANGUAGES C CXX)

# PS Vita builds require the PSP2 (VitaSDK) toolchain.
# Pass -DCMAKE_TOOLCHAIN_FILE=/path/to/vita.toolchain.cmake when configuring.
# See: https://github.com/vitasdk/vita-toolchain

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

# Detect platform and set appropriate defines
if(EMSCRIPTEN)
    # WASM/Emscripten build
    set(PLATFORM_DEFINES _LARGE_WORLDS;_DEBUG_MENUS_ENABLED;_LIB;_CRT_NON_CONFORMING_SWPRINTFS;_CRT_SECURE_NO_WARNINGS;__EMSCRIPTEN__)
    set(PLATFORM_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/WASM/vita_sdk_stubs"
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/PSVitaExtras"
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/inc"
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/Network"
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Generic" OR VITA_TOOLCHAIN)
    # PSVita build
    set(PLATFORM_DEFINES _LARGE_WORLDS;_DEBUG_MENUS_ENABLED;_LIB;_CRT_NON_CONFORMING_SWPRINTFS;_CRT_SECURE_NO_WARNINGS;_PSVita;__PSVITA__;__psp2__)
    set(PLATFORM_INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/PSVitaExtras"
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/4JLibs/inc"
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/Network"
        "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita"
    )
else()
    # Other platforms (Windows, etc.)
    set(PLATFORM_DEFINES _LARGE_WORLDS;_DEBUG_MENUS_ENABLED;_LIB;_CRT_NON_CONFORMING_SWPRINTFS;_CRT_SECURE_NO_WARNINGS)
    set(PLATFORM_INCLUDES "")
endif()

add_library(MinecraftWorld STATIC ${MINECRAFT_WORLD_SOURCES})
target_include_directories(MinecraftWorld PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.World"
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.World/x64headers"
  ${PLATFORM_INCLUDES}
)
target_compile_definitions(MinecraftWorld PRIVATE
  $<$<CONFIG:Debug>:${PLATFORM_DEFINES};_DEBUG>
  $<$<NOT:$<CONFIG:Debug>>:${PLATFORM_DEFINES}>
)
if(MSVC)
  configure_msvc_target(MinecraftWorld)
endif()

add_executable(MinecraftClient ${MINECRAFT_CLIENT_SOURCES})
target_include_directories(MinecraftClient PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client"
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/Iggy/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/PSVita/Sentient"
  "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.World/x64headers"
  ${PLATFORM_INCLUDES}
)
target_compile_definitions(MinecraftClient PRIVATE
  $<$<CONFIG:Debug>:${PLATFORM_DEFINES};_DEBUG>
  $<$<NOT:$<CONFIG:Debug>>:${PLATFORM_DEFINES}>
)
if(MSVC)
  configure_msvc_target(MinecraftClient)
  target_link_options(MinecraftClient PRIVATE
    $<$<CONFIG:Release>:/LTCG /INCREMENTAL:NO>
  )
endif()

target_link_libraries(MinecraftClient MinecraftWorld)

set_target_properties(MinecraftClient MinecraftWorld PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)
