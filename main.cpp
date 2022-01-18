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
  // Create
  if(createWindow("Vulkan", 1280, 720) == EXIT_FAILURE) return EXIT_FAILURE;

#ifdef NDEBUG // Not debug : Disable validation for better performance and size
  createInstance("Triangle", {1,0,0}, "Template_Engine", {1,0,0}, false);
#else // Debug : Enable validation
  createInstance("Triangle", {1,0,0}, "Template_Engine", {1,0,0}, true);
#endif
  setupDebugMessenger();

  // Loop
  while(!g_quitFlag)
  {
    pollEvents();
  }

  // Delete
  if(g_enabledValidationLayers) destroyDebugUtilsMessengerEXT(g_instance, g_debugMessenger, nullptr); // extensions must be destroyed before instance destruction!
  vkDestroyInstance(g_instance, nullptr);
  destroyWindow();

  return EXIT_SUCCESS;
}
