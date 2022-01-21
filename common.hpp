#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cassert>
#include <optional>
#include <set>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

uint32_t g_width = 1;
uint32_t g_height = 1;

bool g_quitFlag = false;
bool g_enabledValidationLayers = false;
bool g_deviceExtensionsSupported = false;

VkInstance g_instance = VK_NULL_HANDLE;
VkDebugUtilsMessengerEXT g_debugMessenger = VK_NULL_HANDLE;
VkSurfaceKHR g_surface = VK_NULL_HANDLE;
VkPhysicalDevice g_physicalDevice = VK_NULL_HANDLE;
VkDevice g_device = VK_NULL_HANDLE;
VkQueue g_graphicsQueue = VK_NULL_HANDLE;
VkQueue g_presentQueue = VK_NULL_HANDLE;
VkCommandPool g_commandPool = VK_NULL_HANDLE;

VkSwapchainKHR g_swapchain = VK_NULL_HANDLE;
std::vector<VkImage> g_swapChainImages;
VkFormat g_swapChainImageFormat;
VkExtent2D g_swapChainExtent;

VkPhysicalDeviceProperties g_properties;

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily; // Making the graphicsFamily's existence testable.
  std::optional<uint32_t> presentFamily; // Present family may not be in the same as graphics, so need to be checked separately
};

#define CHECK_VULKAN_ERRORS(Expression) do { VkResult result = (Expression); if(result < 0) assert(0); } while(0)

extern bool swapChainIsAdequate(VkPhysicalDevice device);
extern QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

