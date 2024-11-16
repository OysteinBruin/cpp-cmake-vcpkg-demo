cmake_minimum_required(VERSION 3.16)
project(cross_platform_example
    VERSION 1.0
    DESCRIPTION "Cross-Platform SDL2 Example"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# Define the executable based on platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_executable(${PROJECT_NAME} WIN32
        src/main.cpp
    )
    
    # Get SDL2 DLL path for Windows
    get_target_property(SDL2_DLL_LOCATION SDL2::SDL2 IMPORTED_LOCATION_RELEASE)
    if(NOT SDL2_DLL_LOCATION)
        get_target_property(SDL2_DLL_LOCATION SDL2::SDL2 IMPORTED_LOCATION)
    endif()
    
    # Copy SDL2 DLL using file(COPY)
    file(COPY "$ENV{VCPKG_ROOT}/installed/x64-mingw-dynamic/bin/SDL2.dll"
         DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
else()
    add_executable(${PROJECT_NAME}
        src/main.cpp
    )
endif()

# Link SDL2 with platform-specific settings
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        -mwindows
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
    )
endif()

# Add include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Platform-specific compile definitions
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        WINDOWS_BUILD
    )
endif()