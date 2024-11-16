cmake_minimum_required(VERSION 3.16)
project(cross_platform_example
    VERSION 1.0
    DESCRIPTION "Cross-Platform SDL2 Example"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define option for targeting Windows
option(TARGET_WINDOWS "Build for Windows target" OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configuration for Windows cross-compilation
if(TARGET_WINDOWS)
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
    
    # Adjust the default behavior of find_xxx() commands
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    
    # Enable Windows subsystem
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    
    # Set vcpkg triplet for Windows
    set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic")
else()
    # Native build settings (Linux)
    set(VCPKG_TARGET_TRIPLET "x64-linux")
endif()

# Include vcpkg toolchain
if(DEFINED ENV{VCPKG_ROOT})
    include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not set")
endif()

# Find SDL2
find_package(SDL2 CONFIG REQUIRED)

# Define the executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Link SDL2
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
)

# Platform-specific configurations
if(TARGET_WINDOWS)
    # Custom command to copy SDL2 DLL without using PowerShell
    add_custom_command(
        TARGET ${PROJECT_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DLL_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copying SDL2 DLL to output directory"
    )
endif()

# Add include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: Add platform-specific compile definitions
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    $<$<PLATFORM_ID:Windows>:WINDOWS_BUILD>
    $<$<PLATFORM_ID:Linux>:LINUX_BUILD>
)