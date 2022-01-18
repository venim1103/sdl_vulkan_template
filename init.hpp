#pragma once

#ifdef __APPLE__
const std::vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"}; // Validation layers enabled
#else
const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"}; // Validation layers enabled
#endif

static bool validationLayersSupported()
{
  uint32_t layerCount, layersFound = 0;

  CHECK_VULKAN_ERRORS( vkEnumerateInstanceLayerProperties(&layerCount, nullptr) ); // Check how many layers can exist
//  std::cout << "layerCount: " << int(layerCount) << std::endl;
  
  std::vector<VkLayerProperties> availableLayers(layerCount); // Make a vector of available layers
  CHECK_VULKAN_ERRORS( vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()) ); // Fill up available layer data

  for(const char* layerName : validationLayers)
  {
    for(const auto& layerProperties : availableLayers) // Check if requested validationLayers are found (available)
    {
      if(strcmp(layerName, layerProperties.layerName) == 0) // Layer found!
      {
        layersFound++;
        break;
      }
    }
  }
  if(layersFound == validationLayers.size()) return true; // Found all requested layers
  return false; // All requested layers found!
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
  std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
  auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if(function != nullptr) return function(instance, pCreateInfo, pAllocator, pDebugMessenger);
  return VK_ERROR_EXTENSION_NOT_PRESENT; // else
}

static void createDebugMessenger(VkInstanceCreateInfo& createInfo)
{
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
  if(g_enabledValidationLayers)
  {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; 
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback; // Set CALLBACK error function!
    debugCreateInfo.pUserData == nullptr; // Optional, no needed

    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    CHECK_VULKAN_ERRORS( createDebugUtilsMessengerEXT(g_instance, &debugCreateInfo, nullptr, &g_debugMessenger) );

  } else
  {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }
}

void createInstance(std::string app_name, std::vector<uint8_t> app_version, std::string engine_name, std::vector<uint8_t> engine_version, bool enableValidationLayers)
{
  if(enableValidationLayers)
  {
    if(validationLayersSupported()) 
    {
      g_enabledValidationLayers = true;
    } else throw std::runtime_error("Validation layers requested, but are not supported!");
  } else g_enabledValidationLayers = false;

  // Create Vulkan Application instance
  VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  appInfo.pApplicationName = app_name.c_str(); // Enter the name
  appInfo.applicationVersion = VK_MAKE_VERSION(app_version[0], app_version[1], app_version[2]); // Enter version
  appInfo.pEngineName = engine_name.c_str(); // Enter the name
  appInfo.engineVersion = VK_MAKE_VERSION(engine_version[0], engine_version[1], engine_version[2]); // Enter version
  appInfo.apiVersion = VK_API_VERSION_1_0; // Vulkan API version
  
  VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO }; // Create the application instance
  createInfo.pApplicationInfo = &appInfo;
  
  // Prepare debug utilities
  std::vector<const char*> extensions = { VK_KHR_SURFACE_EXTENSION_NAME };
#ifdef _WIN32
  extensions.push_back( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
#elif defined(__APPLE__)
  extensions.push_back( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
  extensions.push_back( "VK_KHR_get_physical_device_properties2" ); // TODO: Make correct checking
#elif defined(__linux__)
  extensions.push_back( VK_KHR_XLIB_SURFACE_EXTENSION_NAME );
#endif

  if(g_enabledValidationLayers) extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  // Create the debug messenger
//  createDebugMessenger( createInfo );
  // ... instInfo
  CHECK_VULKAN_ERRORS( vkCreateInstance(&createInfo, nullptr, &g_instance) );
}

int createSurface()
{

  return EXIT_SUCCESS;
}

int createPhysicalDevice()
{

  return EXIT_SUCCESS;
}

int createLogicalDevice()
{

  return EXIT_SUCCESS;
}

int createCommandPool()
{

  return EXIT_SUCCESS;
}

