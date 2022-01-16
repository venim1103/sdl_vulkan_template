VULKAN_SDK_PATH_MAC = $(HOME)/VulkanSDK/1.2.198.1/macOS
VULKAN_SDK_PATH_LINUX = $(HOME)/Downloads/Vulkan/1.2.198.1/x86_64
VULKAN_SDK_PATH_WINDOWS = /e/Vulkan
MINGWSDL2 = /e/SDL2-2.0.20/x86_64-w64-mingw32

MATHFU = mathfu

GLSLC_LIN = $(VULKAN_SDK_PATH_LINUX)/bin/glslc
GLSLC_WIN = $(VULKAN_SDK_PATH_WINDOWS)/Bin/glslc.exe
GLSLC_MAC = $(VULKAN_SDK_PATH_MAC)/bin/glslc

GPP_UNIX = g++
GPP_WIN = x86_64-w64-mingw32-g++

WINDOWSFLAGS = -D_WIN32 -DVK_USE_PLATFORM_WIN32_KHR -U__STRICT_ANSI__

CFLAGS_LINUX = -std=c++17 -I. -I$(VULKAN_SDK_PATH_LINUX)/include -I$(MATHFU)/include
LDFLAGS_LINUX = -L$(VULKAN_SDK_PATH_LINUX)/lib -lSDL2main -lSDL2 -lvulkan

CFLAGS_MAC = -std=c++17 -I. -I$(VULKAN_SDK_PATH_MAC)/include -I$(MATHFU)/include
LDFLAGS_MAC = -L$(VULKAN_SDK_PATH_MAC)/lib -lSDL2main -lSDL2 -lvulkan

CFLAGS_WIN = -std=c++17 -I. -I$(VULKAN_SDK_PATH_WINDOWS)/Include -I$(MINGWSDL2)/include/ -I$(MATHFU)/include
LDFLAGS_WIN = -Wl,-subsystem,windows -mwindows -municode -o a.exe -L$(VULKAN_SDK_PATH_WINDOWS)/Lib -L$(VULKAN_SDK_PATH_WINDOWS)/Bin -L$(MINGWSDL2)/lib -L$(MINGWSDL2)/bin $(WINDOWSFLAGS) -lmingw32 -lvulkan-1 -lSDL2main -lSDL2

lin: *.cpp *.hpp
#	$(GLSLC_LIN) shaders/*.vert -o shaders/vert.spv
#	$(GLSLC_LIN) shaders/*.frag -o shaders/frag.spv
	$(GPP_UNIX) $(CFLAGS_LINUX) -D__linux__ -DVK_USE_PLATFORM_XLIB_KHR -o a.out *.cpp $(LDFLAGS_LINUX)
	cp -rf shaders BUILD/LIN/
	mv a.out BUILD/LIN/a.out

mac: *.cpp *.hpp
#	$(GLSLC_MAC) shaders/*.vert -o shaders/vert.spv
#	$(GLSLC_MAC) shaders/*.frag -o shaders/frag.spv
	$(GPP_UNIX) $(CFLAGS_MAC) -D__APPLE__ -DVK_USE_PLATFORM_MACOS_MVK -DVK_ENABLE_BETA_EXTENSIONS -o a.out *.cpp $(LDFLAGS_MAC)
	cp -rf shaders BUILD/MAC/
	mv a.out BUILD/MAC/a.out

win: *.cpp *.hpp
#	$(GLSLC_WIN) shaders/*.vert -o shaders/vert.spv
#	$(GLSLC_WIN) shaders/*.frag -o shaders/frag.spv
	$(GPP_WIN) $(CFLAGS_WIN) *.cpp $(LDFLAGS_WIN)
	cp -rf shaders BUILD/WIN/
	mv a.exe BUILD/WIN/a.exe

.PHONY: linrun macrun winrun linclean macclean winclean

linrun: lin
	BUILD/LIN/a.out
	rm BUILD/LIN/a.*

macrun: mac
	BUILD/MAC/a.out
	rm BUILD/MAC/a.*

winrun: win
	BUILD/WIN/a.exe
	rm BUILD/WIN/a.*

linclean:
	rm BUILD/LIN/a.*

macclean:
	rm BUILD/MAC/a.*

winclean:
	rm BUILD/WIN/a.*

