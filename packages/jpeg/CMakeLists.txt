cmake_minimum_required(VERSION 3.10)
project(StegoProject C)

# Include local header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Include libjpeg-turbo headers and library paths
include_directories(/opt/homebrew/opt/jpeg-turbo/include)
link_directories(/opt/homebrew/opt/jpeg-turbo/lib)

# Add source files
add_executable(stego
    src/main.c
    src/stego.c
    src/jpeg_utils.c
    src/file_utils.c
)

# Link to libjpeg
target_link_libraries(stego PRIVATE jpeg)

# Enable Address Sanitizer (ASan) for debugging
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")