cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico2)

# pico-sdk
set(PICO_SDK_FETCH_FROM_GIT on)
set(PICO_SDK_GIT_TAG 2.1.1)
include(pico_sdk_import.cmake)

project(wolvsec_badge C ASM CXX)
enable_language(CXX)
pico_sdk_init()

add_executable(wolvsec_badge
    wolvsec_badge.c
    lcd_impl.c
    lvgl/src/drivers/display/st7789/lv_st7789.c
)

target_include_directories(wolvsec_badge PRIVATE
    ${CMAKE_SOURCE_DIR}
    lvgl
    lvgl/src/drivers/display
)

target_compile_definitions(wolvsec_badge PRIVATE LV_CONF_PATH="${CMAKE_SOURCE_DIR}/lv_conf.h")

add_subdirectory(lvgl lvgl_build)
target_link_libraries(wolvsec_badge
    pico_stdlib
    hardware_spi
    hardware_dma
    lvgl
)

# Enable USB stdio for debugging, disable UART stdio
pico_enable_stdio_usb(wolvsec_badge 1)
pico_enable_stdio_uart(wolvsec_badge 0)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(wolvsec_badge pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(wolvsec_badge)
