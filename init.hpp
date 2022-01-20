#pragma once
#include "common.hpp"

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"}; // Validation layers enabled
#ifdef __APPLE__
const std::vector<const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME };
#else
const std::vector<const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
#endif

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily; // Making the graphicsFamily's existence testable.
  std::optional<uint32_t> presentFamily; // Present family may not be in the same as graphics, so need to be checked separately
};


static bool validationLayersSupported()
{
  uint32_t layerCount, layersFound = 0;

  CHECK_VULKAN_ERRORS( vkEnumerateInstanceLayerProperties(&layerCount, nullptr) ); // Check how many layers can exist
  
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
//  std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  std::cerr << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

// Proxy function for external debugUtilsMessenger creation
static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
  auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if(function != nullptr) return function(instance, pCreateInfo, pAllocator, pDebugMessenger);
  return VK_ERROR_EXTENSION_NOT_PRESENT; // else
}

// Proxy function for external debugUtilsMessenger destruction
void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
  auto function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if(function != nullptr) return function(instance, debugMessenger, pAllocator);
}

static void settingsForDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo)
{
  debugCreateInfo = {}; 
  debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; // (| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) Diagnostic messages, warnings for bugs and error information for crashes
  debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; // Enable specification and performance check related messages
  debugCreateInfo.pfnUserCallback = debugCallback; // Set CALLBACK error function!
  debugCreateInfo.pUserData = nullptr; // Optional, not needed right now (pointer can be pointed to specific data/class/structure etc. for testing)
}

static void createDebugMessenger(VkInstanceCreateInfo& createInfo, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo)
{
  if(g_enabledValidationLayers)
  {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    settingsForDebugMessenger(debugCreateInfo);

    // createInfo.pNext = nullptr;
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo; // For debugging: vkCreateInstance and vkDestroyInstance calls.

  } else
  {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }
}

void setupDebugMessenger()
{
  if(!g_enabledValidationLayers) return;
  
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  settingsForDebugMessenger( debugCreateInfo );
  
  CHECK_VULKAN_ERRORS( createDebugUtilsMessengerEXT(g_instance, &debugCreateInfo, nullptr, &g_debugMessenger) );
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
  
  // Prepare extensions
  std::vector<const char*> extensions = { VK_KHR_SURFACE_EXTENSION_NAME };
#ifdef _WIN32
  extensions.push_back( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
#elif defined(__APPLE__)
  extensions.push_back( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
  extensions.push_back( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME ); // TODO: Make correct checking
#elif defined(__linux__)
  extensions.push_back( VK_KHR_XLIB_SURFACE_EXTENSION_NAME );
#endif

  if(g_enabledValidationLayers) extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ); // For enabling callback setup in debugmessenger
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  // Create the debug messenger
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {}; // Initialize here
  createDebugMessenger( createInfo, debugCreateInfo );

  CHECK_VULKAN_ERRORS( vkCreateInstance(&createInfo, nullptr, &g_instance) );
}

int createSurface(SDL_Window* window)
{
  if(!SDL_Vulkan_CreateSurface(window, g_instance, &g_surface))
  {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr); // Count the families

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data()); // Get the valus into vector.
 
  int i = 0;
  VkBool32 presentSupport = false;
  for(const auto& queueFamily : queueFamilies)
  {
    if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphicsFamily = i; // Found "graphics" bit, save index.

    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, g_surface, &presentSupport); // Check of present  supported in the queue.
    if(presentSupport) indices.presentFamily = i; // If supported, then save the index

    if((indices.graphicsFamily.has_value() && indices.presentFamily.has_value())) break; // If the graphicsFamily is real (existing) and the presentFamily also, then stop.
    i++;
  }
  return indices;
}

static bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr); // Count the extensions found
 
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data()); // Get extensions

  std::set<std::string> requiredExtensionShoppingList(deviceExtensions.begin(), deviceExtensions.end());

  for(const auto& extension : availableExtensions) // Go trough and remove found required extensions from list until all found.
  {
    requiredExtensionShoppingList.erase(extension.extensionName);
  }

  return requiredExtensionShoppingList.empty(); // If all required extensions found and crossed over (list empty).
}

static bool deviceIsSuitable(VkPhysicalDevice device)
{
  // ADD HERE OTHER REQUIREMENTS FUNCTIONS FOR CHOOSING THE GPU, NOW ONLY QUEUE FAMILIES WILL BE CHECKED:
//   VkPhysicalDeviceProperties deviceProperties;
//   VkPhysicalDeviceFeatures deviceFeatures;
//   vkGetPhysicalDeviceProperties(device, &deviceProperties);
//   vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
//   if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader) // EXAMPLE!
  //...

// Queue families are separate queues (command queues) for different tasks: "compute command", "memory transfer", "graphics"... 
  // We need to find a queue family for "graphics" tasks at least (and that our SDL surface is supported:
  
  QueueFamilyIndices indices = findQueueFamilies(device);
  g_deviceExtensionsSupported = checkDeviceExtensionSupport(device);
  return (indices.graphicsFamily.has_value() && indices.presentFamily.has_value() && g_deviceExtensionsSupported && swapChainIsAdequate(device)); // Return true if the graphicsFamily is real (exists) and make sure device extensions are supported.
}

int createPhysicalDevice()
{
  uint32_t deviceCount = 0;
  CHECK_VULKAN_ERRORS( vkEnumeratePhysicalDevices(g_instance, &deviceCount, nullptr) );
  if(deviceCount == 0) throw std::runtime_error("Failed to find any GPU with Vulkan support!");

  std::vector<VkPhysicalDevice> devices(deviceCount);

  CHECK_VULKAN_ERRORS( vkEnumeratePhysicalDevices(g_instance, &deviceCount, devices.data()) );

  for(const auto& device : devices) // For all the devices check which one(s) meet the requirements
  {
    if(deviceIsSuitable(device))
    {
      g_physicalDevice = device;
      break;
    }
  }

  if(g_physicalDevice == VK_NULL_HANDLE) throw std::runtime_error("Failed to find a suitable GPU!");

  vkGetPhysicalDeviceProperties(g_physicalDevice, &g_properties);
  std::cout << "Physical device: " << g_properties.deviceName << std::endl;

  return EXIT_SUCCESS;
}

int createLogicalDevice()
{
  QueueFamilyIndices indices = findQueueFamilies(g_physicalDevice); // Collect the quefamilies from the selected device(s)

  // Create graphics and present queues:
  float queuePriority = 1.0f; // Highest priority for only one queue

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos; // Two queue create infos
  std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() }; // set of indeces
  for(const auto& queueFamily : uniqueQueueFamilies)
  {
    VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO }; 
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1; // Now only one (in general, not many needed as threading is supported)
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo); // Save unique queue into the vector
  }
  
  VkPhysicalDeviceFeatures deviceFeatures = {}; // Disable all feature for now
  
  VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
  createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()); // Graphics and present queues
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  // Below may not needed, included for legacy support: (Instance and device validation layers no need to separate anymore)  
  if(g_enabledValidationLayers) 
  {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else createInfo.enabledLayerCount = 0;

  CHECK_VULKAN_ERRORS( vkCreateDevice(g_physicalDevice, &createInfo, nullptr, &g_device ) );

  // Queues are automatically created when logical devices are created, thus we just need to connect the handles:
  vkGetDeviceQueue(g_device, indices.graphicsFamily.value(), 0, &g_graphicsQueue); // These queue handles could point same place
  vkGetDeviceQueue(g_device, indices.presentFamily.value(), 0, &g_presentQueue); // These queue handles could point same place

  return EXIT_SUCCESS;
}

int createCommandPool()
{
  QueueFamilyIndices indices = findQueueFamilies(g_physicalDevice);
  VkCommandPoolCreateInfo commandPoolCreateInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
  commandPoolCreateInfo.queueFamilyIndex = indices.graphicsFamily.value(); // Command pool connects buffers (memory) to only graphics related queues.
  // Transient: Command buffers rerecorded "often", Reset: Allow command buffers to be rerecorded "individually".
//  commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  commandPoolCreateInfo.pNext = NULL;
  commandPoolCreateInfo.flags = 0;

  CHECK_VULKAN_ERRORS( vkCreateCommandPool(g_device, &commandPoolCreateInfo, nullptr, &g_commandPool) );

  return EXIT_SUCCESS;
}

