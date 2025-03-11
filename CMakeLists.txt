# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Add LVGL library path
set(LVGL_DIR "${CMAKE_SOURCE_DIR}/lvgl")

project(rp2350-lcd-example C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(rp2350-lcd-example
    rp2350-lcd-example.c
    lcd_impl.c
    )

# pull in common dependencies
target_link_libraries(rp2350-lcd-example pico_stdlib hardware_spi)

# enable usb output, disable uart output
pico_enable_stdio_usb(rp2350-lcd-example 1)
pico_enable_stdio_uart(rp2350-lcd-example 0)

# LVGL lib
target_include_directories(rp2350-lcd-example PRIVATE ${CMAKE_SOURCE_DIR})
target_sources(rp2350-lcd-example PRIVATE ${LVGL_DIR}/src/drivers/display/st7789/lv_st7789.c)
target_include_directories(rp2350-lcd-example PRIVATE ${LVGL_DIR}/src/drivers/display)
add_compile_definitions(LV_CONF_PATH="${CMAKE_SOURCE_DIR}/lv_conf.h") # because lvgl could not find lv_conf for some reason


target_include_directories(rp2350-lcd-example PRIVATE ${LVGL_DIR})
add_subdirectory(${LVGL_DIR} lvgl_build)
target_link_libraries(rp2350-lcd-example lvgl)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(rp2350-lcd-example pico_cyw43_arch_none)
endif()

# create map/bin/hex file etc.
pico_add_extra_outputs(rp2350-lcd-example)

