#include "common.hpp"
#include "sdl.hpp"
#include "init.hpp"
#include "swap.hpp"

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
  if(createSurface(g_window) == EXIT_FAILURE) return EXIT_FAILURE;
  createPhysicalDevice();
  createLogicalDevice();
  createCommandPool();
  createSwapChain();

  // Loop
  while(!g_quitFlag)
  {
    pollEvents();
  }

  // Delete
  vkDestroyCommandPool(g_device, g_commandPool, nullptr);
  g_commandPool = VK_NULL_HANDLE;

  if(g_enabledValidationLayers) destroyDebugUtilsMessengerEXT(g_instance, g_debugMessenger, nullptr); // extensions must be destroyed before instance destruction
  g_debugMessenger = VK_NULL_HANDLE;

  vkDestroySwapchainKHR(g_device, g_swapchain, nullptr);
  g_swapchain = VK_NULL_HANDLE;

  vkDestroySurfaceKHR(g_instance, g_surface, nullptr);
  g_surface = VK_NULL_HANDLE;

  vkDestroyDevice(g_device, nullptr);
  g_device = VK_NULL_HANDLE;

  vkDestroyInstance(g_instance, nullptr);
  g_instance = VK_NULL_HANDLE;

  g_graphicsQueue = VK_NULL_HANDLE;
  g_presentQueue = VK_NULL_HANDLE;
  g_physicalDevice = VK_NULL_HANDLE;

  destroyWindow();

  return EXIT_SUCCESS;
}

