cmake_minimum_required(VERSION 3.10)
project(StegoProject C)

# Manually set the paths
include_directories(/opt/homebrew/opt/jpeg-turbo/include)
link_directories(/opt/homebrew/opt/jpeg-turbo/lib)

add_executable(stego src/main.c)

# Link directly
target_link_libraries(stego PRIVATE -ljpeg)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")