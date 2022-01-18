# sdl_vulkan_template
Basic Vulkan app template using SDL for creating a triangle.

## Basic Setup
Inside the working folder: 
- git clone https://github.com/google/mathfu.git
### VulkanMemoryAllocator:
- git clone https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
- git checkout tags/v2.3.0 // or newest release!
- cp VulkanMemoryAllocator/src/vk_mem_alloc.h ./ // copy the headerfile to project root!

## Windows: 
- Install SDL2 for Mingw: [https://www.libsdl.org/index.php](https://www.libsdl.org/index.php) into your desired $(MINGWSDL2) folder and update Makefile accordingly
- Install vulkan-sdk: [https://vulkan.lunarg.com/](https://vulkan.lunarg.com/) into your desired $(VULKAN_SDK_PATH_WINDOWS) folder and update Makefile accordingly
- Install msys2 [https://www.msys2.org/](https://www.msys2.org/) + sdl2 + mingw-w64-x86_64-vulkan-headers into your desired $(MSYS2_INSTALLATION_PATH): [https://gist.github.com/thales17/fb2e4cff60890a51d9dddd4c6e832ad2](https://gist.github.com/thales17/fb2e4cff60890a51d9dddd4c6e832ad2)
### Inside MSYS2 (MinGW x64): 
- pacman -Syu
- pacman -S git mingw-w64-x86_64-toolchain mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_mixer mingw64/mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_ttf mingw64/mingw-w64-x86_64-SDL2_net make mingw-w64-x86_64-vulkan-headers
- cp $(MSYS2_INSTALLATION_PATH)/mingw64/bin/libgcc_s_seh-1.dll BUILD/WIN/
- cp $(MSYS2_INSTALLATION_PATH)/mingw64/bin/libstdc++-6.dll BUILD/WIN/
- cp $(MSYS2_INSTALLATION_PATH)/mingw64/bin/libwinpthread-1.dll BUILD/WIN/
- cp $(MINGWSDL2)/x86_64-w64-mingw32/bin/SDL2.dll BUILD/WIN/
- ... (any other missing dll for getting the .exe to run)

## Mac specific:
- Install Xcode from AppStore
- Inside command line: xcode-select --install
- Install vulkan-sdk: [https://vulkan.lunarg.com/](https://vulkan.lunarg.com/) into your desired $(VULKAN_SDK_PATH_MAC) folder and update Makefile accordingly
- Install brew: [https://brew.sh](https://brew.sh)
### Inside command line
Install development libraries:
- brew update && brew upgrade && brew clean
- brew install gcc g++ make sdl2 sdl2_mixer sd2l_image sdl2_ttf sdl2_net

## Linux specific:
- Install vulkan-sdk: [https://vulkan.lunarg.com/](https://vulkan.lunarg.com/) into your desired $(VULKAN_SDK_PATH_LINUX) folder and update Makefile accordingly
### Inside command line
Install development libraries:
- sudo apt update && sudo apt upgrade
- sudo apt install build-essential gcc g++ make libsdl2-dev libsdl2-mixer-dev libsd2l-image-dev libsdl2-ttf-dev libsdl2-net-dev

# Compilation options:
- make win
- make lin
- make mac
- make winclean
- make linclean
- make macclean
- make winrun
- make linrun
- make macrun

