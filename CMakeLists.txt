# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.5)

set(FAMILY rp2040)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)
include($ENV{PICO_SDK_PATH}/lib/tinyusb/hw/bsp/family_support.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(th8a C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME})

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_host
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_sources(${PROJECT_NAME}  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/hid_app.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tusb_config.h
    ${CMAKE_CURRENT_LIST_DIR}/src/gears.h
    ${CMAKE_CURRENT_LIST_DIR}/src/gears.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/src/handbrake.h
    # ${CMAKE_CURRENT_LIST_DIR}/src/handbrake.cpp
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/src
        # ${CMAKE_CURRENT_LIST_DIR}/pico-pio-usb
        # ${CMAKE_CURRENT_LIST_DIR}/pico-pio-usb/src
    )

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_i2c
    tinyusb_host
    tinyusb_board
)

# Configure compilation flags and libraries for the example... see the corresponding function
# in hw/bsp/FAMILY/family.cmake for details.
# family_configure_host_example(${PROJECT_NAME})


# Disable usb output, enable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)