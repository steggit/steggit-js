project(NitroSteggit)
cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME NitroSteggit)
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 20)

# Path to the core package (relative to this CMakeLists.txt)
set(CORE_PATH "${CMAKE_SOURCE_DIR}/../../core")

# Find required libraries
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

# Define C library with core steggit sources - reference directly from core package
add_library(steggit_core STATIC
        ${CORE_PATH}/src/general_utils.c
        ${CORE_PATH}/src/jpeg_utils.c
        ${CORE_PATH}/src/png_utils.c
        ${CORE_PATH}/src/main.c
        # Note: Excluding wrapper_utils.c and wrapper.c as they contain Node.js N-API specific code
)

# Set up includes for core library - reference headers directly from core package
target_include_directories(steggit_core PUBLIC
        ${CORE_PATH}/include
        ${PNG_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIRS}
)

# Link core library dependencies
target_link_libraries(steggit_core
        ${PNG_LIBRARIES}
        ${JPEG_LIBRARIES}
        z
        m
)

# Define C++ library and add all sources
add_library(${PACKAGE_NAME} SHARED
        src/main/cpp/cpp-adapter.cpp
)

# Add Nitrogen specs :)
include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/NitroSteggit+autolinking.cmake)

# Set up local includes
include_directories(
        "src/main/cpp"
        "../cpp"
        ${CORE_PATH}/include
        ${PNG_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIRS}
)

find_library(LOG_LIB log)

# Link all libraries together
target_link_libraries(
        ${PACKAGE_NAME}
        ${LOG_LIB}
        android                                   # <-- Android core
        steggit_core
        ${PNG_LIBRARIES}
        ${JPEG_LIBRARIES}
)
