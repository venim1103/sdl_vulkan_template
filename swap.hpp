#pragma once
#include "common.hpp"

// Swap chains require these properties:
// Basic surface properties: minmax number of images / swap chain, minmax width and height of images..
// Supported surface formats: Pixel format, color space..
// Available presentation modes.

struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails getSwapChainSupportDetails(VkPhysicalDevice device)
{
  SwapChainSupportDetails details;

  // Get the surface capabilities:
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, g_surface, &details.capabilities);

  // Get supported surface formats:
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, g_surface, &formatCount, nullptr); // Get how many supported
  if(formatCount != 0) // If found some formats (more than nothing)
  {
    details.formats.resize(formatCount); // resize the vector
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, g_surface, &formatCount, details.formats.data()); // Save the gotten formats
  }

  // Get supported present modes:
  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, g_surface, &presentModeCount, nullptr); // Get how many supported
  if(presentModeCount != 0) // If found some present modes (more than nothing)
  {
    details.presentModes.resize(presentModeCount); // resize the vector vkGetPhysicalDeviceSurfacePresentModesKHR(device, g_surface, &presentModeCount, details.presentModes.data()); // Save the gotten present modes
  }
  return details;
}

bool swapChainIsAdequate(VkPhysicalDevice device)
{ // Suffiecient for now when at least one: supported image format, supported presentation mode / given surface.
  bool swapChainSuitability = false;
  if(g_deviceExtensionsSupported)
  {
    SwapChainSupportDetails swapChainSupport = getSwapChainSupportDetails(device);
    // Suitable when the list of suitable formats and present modes are not empty.
    swapChainSuitability = (!swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty());
  }
  return swapChainSuitability;
}

