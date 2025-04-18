cmake_minimum_required(VERSION 3.10)
project(StegoProject C)

# Create compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include local header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Include libjpeg-turbo headers and library paths
include_directories(/opt/homebrew/opt/jpeg-turbo/include)
link_directories(/opt/homebrew/opt/jpeg-turbo/lib)

# Include libpng headers and library paths
include_directories(/opt/homebrew/opt/libpng/include)
link_directories(/opt/homebrew/opt/libpng/lib)

# Include the Node API headers
include_directories(/opt/homebrew/include/node)

# Add source files
add_executable(stego
    src/main.c
    src/general_utils.c
    src/jpeg_utils.c
    src/png_utils.c
)

# Link to libjpeg and libpng
target_link_libraries(stego PRIVATE jpeg png)

# Enable Address Sanitizer (ASan) for debugging
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# ---- Test Setup ----
# Add Unity as a library
add_subdirectory(third_party/unity)

# Add test executable
file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c")
add_executable(test_runner
    ${TEST_FILES}
    src/png_utils.c
    src/jpeg_utils.c
    src/general_utils.c
)

# Include fixtures
add_compile_definitions(FIXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures")

# Include test dependencies
target_include_directories(test_runner PRIVATE
    src
    third_party/unity/src
)

# Link libs to the test target
target_link_libraries(test_runner PRIVATE unity jpeg png)