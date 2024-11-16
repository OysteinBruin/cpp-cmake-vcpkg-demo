## Cross compilation from Linux to Windows uaing CMake and vcpkg
Cross-compilation from Linux to Windows using CMake, vcpkg, and SDL2. Cross-compiling for Windows from Linux requires using MinGW-w64 (Minimalist GNU for Windows) as your toolchain.
Here's a step-by-step approach:

1. First, install the required tools:

```bash
sudo apt-get update
sudo apt-get install mingw-w64
```

2. Next, configure vcpkg for cross-compilation. Create a custom triplet file for Windows x64 target. This should be saved as `x64-mingw-dynamic.cmake` in your vcpkg/triplets directory:

```cmake
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_CMAKE_SYSTEM_NAME MinGW)
```

3. Install SDL2 for Windows using vcpkg:

```bash
./vcpkg install sdl2:x64-mingw-dynamic
```

4. Use a basic CMake toolchain file for MinGW cross-compilation. Saved as `mingw-w64-toolchain.cmake` in the root folder.

Some important things to note:

- Make sure all your dependencies are installed for the Windows target through vcpkg
- The resulting executable will have a .exe extension
- You'll need to distribute the necessary DLLs with your executable
- SDL2 specifically requires its DLL to be in the same directory as the executable

## Setup for cross compilation
1. Make the build scripts executable:
```bash
chmod +x build-linux.sh build-windows.sh
```

2. Build for Linux: `./build-linux.sh`

3. Build for Windows: `./build-windows.sh`

The builds will be placed in separate directories:

- `build-linux/bin/` - Linux executable
- `build-windows/bin/` - Windows executable (.exe) with required DLLs

You can also use conditional compilation in your code if needed:
```cpp
#ifdef WINDOWS_BUILD
    // Windows-specific code
#endif

#ifdef LINUX_BUILD
    // Linux-specific code
#endif
```

Key features of this setup:

1. Single CMakeLists.txt file handling both platforms
2. Separate build directories for each target
3. Proper handling of platform-specific dependencies
4. Automatic DLL copying for Windows
5. Platform-specific compile definitions if needed

Common practices to remember:

1. Always use forward slashes in CMake paths, even on Windows
2. Use generator expressions ($<...>) for platform-specific settings
3. Keep platform-specific code minimal and isolated
4. Use vcpkg for dependency management on both platforms
5. Maintain separate build directories for different targets