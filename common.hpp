#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

uint32_t g_width = 1;
uint32_t g_height = 1;

bool g_quitFlag = false;

#define CHECK_VULKAN_ERRORS(Expression) do { VkResult result = (Expression); if(result < 0) assert(0); } for(;;)

