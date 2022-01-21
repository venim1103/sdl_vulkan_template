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

static SwapChainSupportDetails getSwapChainSupportDetails(VkPhysicalDevice device)
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

// Choose color format!
static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
  for(const auto& available : availableFormats)
  {
    if(available.format == VK_FORMAT_B8G8R8A8_SRGB && available.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return available;
  }
  return availableFormats[0]; // If not available, return whatever we have (first).
}
// Choose buffer type in syncing with the screen (refresh)!
static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
  for(const auto& presentMode : availablePresentModes)
  {
//  if(presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) return presentMode; // Image --directly-> screen : May cause tearing!
    if(presentMode == VK_PRESENT_MODE_FIFO_KHR) return presentMode; // FIFO (push@refresh, pop2screen): full wait 4 sync ~= VSync!
//  if(presentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) return presentMode; // Similar + If queue late, but was empty ->pop: Tear!
//  if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR) return presentMode; // Circular: replace old=new: "Triple buffering", no rest!
  }
  return VK_PRESENT_MODE_FIFO_KHR; // Should be always supported, thus a good backup. Works also good in mobile (more rest!)!
}
// Choose settings to turn Window resolution to "Vulkan swap chain images in pixel space" resolution.
static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
  if(surfaceCapabilities.currentExtent.width != UINT32_MAX) return surfaceCapabilities.currentExtent;
  // else
  VkExtent2D result
  {
    std::max( surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, g_width) ),
    std::max( surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, g_height) )
  };
  return result;
}

void createSwapChain()
{
  // Choose settings accoring to the saved SwapChainSupportDetails
  SwapChainSupportDetails swapChainSupport = getSwapChainSupportDetails(g_physicalDevice);
  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  // Images inside swap chain should include one extra to reduce waiting (latency) when rendering.
  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  // ImageCount should not exceed max image amount (maxImageCount = 0 means a max image amount is not set!)
  if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) imageCount = swapChainSupport.capabilities.maxImageCount;

  VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
  createInfo.surface = g_surface;
  createInfo.minImageCount = imageCount; // Set the max image count per swap chain
  createInfo.imageFormat = surfaceFormat.format; // From the settings above
  createInfo.imageColorSpace = surfaceFormat.colorSpace; // From the settings above
  createInfo.imageExtent = extent; // From the settings above
  createInfo.imageArrayLayers = 1; // CHANGE THIS IF WANT TO HAVE MULTIPLE LAYERS FOR EXAMPLE STEREOSCOPIC 3D VR SCREEN!
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // No post processing (attach directly): Check other options (VK_IMAGE_USAGE_TRANSFER_DST_BIT can use memory operation to transfer rendered image to swap chain image etc..)!!

  // Next handle swap chain images used in multiple different queue families:
  QueueFamilyIndices indices = findQueueFamilies(g_physicalDevice);
  uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
  if(indices.graphicsFamily != indices.presentFamily) // If they are not in the same family: Have to "concurrently" give settings
  { // Other options available but require "ownership specific" settings, which may be handled later.
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Images used across families with no ownership transfers.
    createInfo.queueFamilyIndexCount = 2; // Two separate ones
    createInfo.pQueueFamilyIndices = queueFamilyIndices; // Pointer to the families (indices).
  } else // EXCLUSIVE mode means each image owned by one family at a time -> Good performance!
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0; // For exclusive this is optional
    createInfo.pQueueFamilyIndices = nullptr; // Optional
  }
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform; // No transform, could use rotations or flipping etc.
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Ignore the alpha channel for now.
  createInfo.presentMode = presentMode; // From the settings above
  createInfo.clipped = VK_TRUE; // Enable clipping! 

  createInfo.oldSwapchain = VK_NULL_HANDLE; // As we only have one swap chain for now, ignore (needed when recreating swapchain!)
  // Create the swap chain:
  CHECK_VULKAN_ERRORS( vkCreateSwapchainKHR(g_device, &createInfo, nullptr, &g_swapchain) );
  // Get and save information: (Images' memory is automatically handled from creating and destroying the swap chain itself!)
  vkGetSwapchainImagesKHR(g_device, g_swapchain, &imageCount, nullptr); // Check how actual images inside
  g_swapChainImages.resize(imageCount); // Update the size
  vkGetSwapchainImagesKHR(g_device, g_swapchain, &imageCount, g_swapChainImages.data()); // Get the images, save to global vector
  g_swapChainImageFormat = surfaceFormat.format; // Save to global values
  g_swapChainExtent = extent; // Save to gloval values
}

