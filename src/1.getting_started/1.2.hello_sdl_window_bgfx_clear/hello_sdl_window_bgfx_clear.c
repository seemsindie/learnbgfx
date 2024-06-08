#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdbool.h>
#include <bgfx/c99/bgfx.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <setup_metal_layer.h>

int main(int argc, char* argv[]) {
  uint32_t width = 800;
  uint32_t height = 600;
  uint32_t debug = BGFX_DEBUG_TEXT;
  uint32_t reset = BGFX_RESET_VSYNC;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Failed to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window* window =
      SDL_CreateWindow("Hello SDL Window", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Failed to create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_SysWMinfo wmi;
  SDL_VERSION(&wmi.version);
  if (!SDL_GetWindowWMInfo(window, &wmi)) {
    printf("Failed to get window info: %s\n", SDL_GetError());
    return 1;
  }

  bgfx_init_t init;
  bgfx_init_ctor(&init);

  #if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  init.platformData.ndt = wmi.info.x11.display;
  init.platformData.nwh = (void*)(uintptr_t)wmi.info.x11.window;
  init.type = BGFX_RENDERER_TYPE_OPENGL;
  #elif BX_PLATFORM_OSX
  init.platformData.nwh = setup_metal_layer((void*)wmi.info.cocoa.window);
  init.type = BGFX_RENDERER_TYPE_METAL;
  #elif BX_PLATFORM_WINDOWS
  init.platformData.nwh = wmi.info.win.window;
  init.type = BGFX_RENDERER_TYPE_DIRECT3D11;
  #endif

  init.resolution.width = width;
  init.resolution.height = height;
  init.resolution.reset = reset;
  
  if (!bgfx_init(&init)) {
    printf("Failed to initialize bgfx\n");
    return 1;
  }

  bgfx_set_debug(debug);

  bgfx_reset(width, height, reset, init.resolution.format);
  bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
  
  SDL_Event event;
  bool running = true;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    bgfx_set_view_rect(0, 0, 0, width, height);

    bgfx_dbg_text_printf(0, 0, 0x4f, "Hello, bgfx!");

    bgfx_touch(0);
    bgfx_frame(false);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}