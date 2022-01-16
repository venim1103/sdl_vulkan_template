#include "common.hpp"
#include "init.hpp"
#include "sdl.hpp"

#ifdef _WIN32
#include <windows.h>
int wmain()
#else
int main()
#endif
{
  if(createWindow("Vulkan", 1280, 720) == EXIT_FAILURE) return EXIT_FAILURE;

  while(!g_quitFlag)
  {
    pollEvents();
  }

  destroyWindow();

  return EXIT_SUCCESS;
}
