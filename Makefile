VULKAN_SDK_PATH_MAC = $(HOME)/VulkanSDK/1.2.198.1/macOS
VULKAN_SDK_PATH_LINUX = $(HOME)/Downloads/Vulkan/1.2.198.1/x86_64
VULKAN_SDK_PATH_WINDOWS = /e/Vulkan
MINGWSDL2 = /e/SDL2-2.0.20/x86_64-w64-mingw32

MACSDL2 = /usr/local/include/
MAC_SDL_LIB_PATH = /usr/local/lib/

MATHFU = mathfu

GLSLC_LIN = $(VULKAN_SDK_PATH_LINUX)/bin/glslc
GLSLC_WIN = $(VULKAN_SDK_PATH_WINDOWS)/Bin/glslc.exe
GLSLC_MAC = $(VULKAN_SDK_PATH_MAC)/bin/glslc

GPP_LINUX = g++ -g
GPP_MAC = /usr/local/bin/g++-11 -g
GPP_WIN = x86_64-w64-mingw32-g++ -g

WINDOWSFLAGS = -D_WIN32 -DVK_USE_PLATFORM_WIN32_KHR -U__STRICT_ANSI__

CFLAGS_LINUX = -std=c++17 -I. -I$(VULKAN_SDK_PATH_LINUX)/include -I$(MATHFU)/include
LDFLAGS_LINUX = -L$(VULKAN_SDK_PATH_LINUX)/lib -lSDL2main -lSDL2 -lvulkan

CFLAGS_MAC = -std=c++17 -I. -I$(VULKAN_SDK_PATH_MAC)/include -I$(MATHFU)/include -I$(MACSDL2)
#LDFLAGS_MAC = -L$(VULKAN_SDK_PATH_MAC)/lib -lSDL2main -lSDL2 -lvulkan
LDFLAGS_MAC = -L$(VULKAN_SDK_PATH_MAC)/lib -L$(MAC_SDL_LIB_PATH) -lSDL2main -lSDL2 -lvulkan

CFLAGS_WIN = -std=c++17 -I. -I$(VULKAN_SDK_PATH_WINDOWS)/Include -I$(MINGWSDL2)/include/ -I$(MATHFU)/include
LDFLAGS_WIN = -Wl,-subsystem,windows -mwindows -municode -o a.exe -L$(VULKAN_SDK_PATH_WINDOWS)/Lib -L$(VULKAN_SDK_PATH_WINDOWS)/Bin -L$(MINGWSDL2)/lib -L$(MINGWSDL2)/bin $(WINDOWSFLAGS) -lmingw32 -lvulkan-1 -lSDL2main -lSDL2

lin: *.cpp *.hpp
#	$(GLSLC_LIN) shaders/*.vert -o shaders/vert.spv
#	$(GLSLC_LIN) shaders/*.frag -o shaders/frag.spv
	$(GPP_LINUX) $(CFLAGS_LINUX) -D__linux__ -DVK_USE_PLATFORM_XLIB_KHR -o a.out *.cpp $(LDFLAGS_LINUX)
	cp -rf shaders BUILD/LIN/
	cp -rf ./*.dSYM BUILD/LIN/
	rm -rf ./*.dSYM
	mv a.out BUILD/LIN/a.out

mac: *.cpp *.hpp
#	$(GLSLC_MAC) shaders/*.vert -o shaders/vert.spv
#	$(GLSLC_MAC) shaders/*.frag -o shaders/frag.spv
	$(GPP_MAC) $(CFLAGS_MAC) -D__APPLE__ -DVK_USE_PLATFORM_MACOS_MVK -DVK_ENABLE_BETA_EXTENSIONS -o a.out *.cpp $(LDFLAGS_MAC)
	cp -rf shaders BUILD/MAC/
	cp -rf ./*.dSYM BUILD/MAC/
	rm -rf ./*.dSYM
	mv a.out BUILD/MAC/a.out

win: *.cpp *.hpp
#	$(GLSLC_WIN) shaders/*.vert -o shaders/vert.spv
#	$(GLSLC_WIN) shaders/*.frag -o shaders/frag.spv
	$(GPP_WIN) $(CFLAGS_WIN) *.cpp $(LDFLAGS_WIN)
	cp -rf shaders BUILD/WIN/
	cp -rf ./*.dSYM BUILD/WIN/
	rm -rf ./*.dSYM
	mv a.exe BUILD/WIN/a.exe

.PHONY: linrun macrun winrun linclean macclean winclean

linrun: lin
	BUILD/LIN/a.out
	rm -rf BUILD/LIN/a.*

macrun: mac
	BUILD/MAC/a.out
	rm -rf BUILD/MAC/a.*

winrun: win
	BUILD/WIN/a.exe
	rm -rf BUILD/WIN/a.*

linclean:
	rm -rf BUILD/LIN/a.*

macclean:
	rm -rf BUILD/MAC/a.*

winclean:
	rm -rf BUILD/WIN/a.*

