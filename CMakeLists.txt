# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)             

set(PICO_BOARD pico_w)
set(FREERTOS_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS)

include(pico_sdk_import.cmake)
include(${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(embarcatech-freertos-tarefa-1 C CXX)

pico_sdk_init()

add_executable(embarcatech-freertos-tarefa-1
    src/main.c
)

target_include_directories(embarcatech-freertos-tarefa-1 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${FREERTOS_PATH}/include          # para IntelliSense
)

target_link_libraries(embarcatech-freertos-tarefa-1
    pico_stdlib
    FreeRTOS-Kernel-Heap4
)

pico_enable_stdio_usb(embarcatech-freertos-tarefa-1 1)
pico_add_extra_outputs(embarcatech-freertos-tarefa-1)
