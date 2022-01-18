#pragma once
#include "common.hpp"

SDL_Window* g_window;

int createWindow(std::string window_name, uint32_t width, uint32_t height)
{
  const uint32_t window_setting_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN;
  const uint32_t centered = SDL_WINDOWPOS_CENTERED;

  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
  if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0 )
  {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  g_window = SDL_CreateWindow(window_name.c_str(), centered, centered, width, height, window_setting_flags);
  if(!g_window)
  {
    std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }
  g_width = width;
  g_height = height;
  return EXIT_SUCCESS;
}

static void handlePossibleResizeEvent(const SDL_Event &event)
{
  if(event.window.event == SDL_WINDOWEVENT_RESIZED)
  {
    uint32_t new_width = event.window.data1;
    uint32_t new_height = event.window.data2;
    if( (new_width > 0) && (new_height > 0) && ( (new_width != g_width) || (new_height != g_height) ) )
    {
      g_width = new_width;
      g_height = new_height;
      // recreateSwapChain();
    }
  }
}

static void handleKeyDown(const SDL_KeyboardEvent &key_event)
{
  if(key_event.keysym.sym == SDLK_ESCAPE) g_quitFlag = true;
}

void pollEvents()
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_EventType::SDL_QUIT:
        g_quitFlag = true;
        break;
      case SDL_EventType::SDL_WINDOWEVENT:
        handlePossibleResizeEvent(event);
        break;
      case SDL_EventType::SDL_KEYDOWN:
        handleKeyDown(event.key);
        break;
      default:
        break;
    }
  }
}

int destroyWindow()
{
  SDL_DestroyWindow(g_window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
