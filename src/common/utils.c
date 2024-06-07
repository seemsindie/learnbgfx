#include <utils.h>

SDL_Window* init_sdl_bgfx(uint32_t width, uint32_t height, bgfx_init_t* init,
                          uint32_t debug, uint32_t reset) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Failed to initialize SDL: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_Window* window = SDL_CreateWindow(
      "Hello SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width,
      height,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  if (window == NULL) {
    printf("Failed to create window: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_SysWMinfo wmi;
  SDL_VERSION(&wmi.version);
  if (!SDL_GetWindowWMInfo(window, &wmi)) {
    printf("Failed to get window info: %s\n", SDL_GetError());
    return NULL;
  }

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  init.platformData.ndt = wmi.info.x11.display;
  init.platformData.nwh = (void*)(uintptr_t)wmi.info.x11.window;
  init.type = BGFX_RENDERER_TYPE_OPENGL;
#elif BX_PLATFORM_OSX
  // init.platformData.nwh = setup_metal_layer((void*)wmi.info.cocoa.window);
  // init.type = BGFX_RENDERER_TYPE_METAL;
  init->platformData.nwh = setup_metal_layer((void*)wmi.info.cocoa.window);
  // init->type = BGFX_RENDERER_TYPE_METAL;
  init->type = BGFX_RENDERER_TYPE_METAL;
#elif BX_PLATFORM_WINDOWS
  init.platformData.nwh = wmi.info.win.window;
  init.type = BGFX_RENDERER_TYPE_DIRECT3D11;
#endif

  init->resolution.width = width;
  init->resolution.height = height;
  init->resolution.reset = reset;

  if (!bgfx_init(init)) {
    printf("Failed to initialize bgfx\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return NULL;
  }

  bgfx_set_debug(debug);

  bgfx_reset(width, height, reset, init->resolution.format);
  bgfx_set_view_clear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f,
                      0);

  return window;
}

void process_input(SDL_Event* event, bool* running, uint32_t* width,
                   uint32_t* height, uint32_t reset, bgfx_init_t init) {
  switch (event->type) {
    case SDL_QUIT:
      *running = false;
      break;
    case SDL_WINDOWEVENT:
      switch (event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
          *width = event->window.data1;
          *height = event->window.data2;
          bgfx_reset(*width, *height, reset, init.resolution.format);
          bgfx_set_view_rect(0, 0, 0, *width, *height);
          break;
      }
      break;
  }
}

bgfx_shader_handle_t load_shader(const char* file_path) {
  FILE* file = fopen(file_path, "rb");

  if (!file) {
    fprintf(stderr, "Failed to open file %s\n", file_path);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  if (size == -1) {
    fprintf(stderr, "Failed to determine file size\n");
    exit(1);
  }
  printf("File size: %ld\n", size);
  fseek(file, 0, SEEK_SET);

  const bgfx_memory_t* mem = bgfx_alloc(size);
  fread(mem->data, 1, size, file);
  fclose(file);
  bgfx_shader_handle_t handle = bgfx_create_shader(mem);

  if (!BGFX_HANDLE_IS_VALID(handle)) {
    fprintf(stderr, "Failed to create shader from file %s\n", file_path);
    exit(1);
  }

  return handle;
}

Shader load_shaders_bin(const char* vertex_shader_path,
                        const char* fragment_shader_path) {
  Shader shader;
  shader.vertex_shader = load_shader(vertex_shader_path);
  shader.fragment_shader = load_shader(fragment_shader_path);
  shader.program =
      bgfx_create_program(shader.vertex_shader, shader.fragment_shader, true);

  if (!BGFX_HANDLE_IS_VALID(shader.program)) {
    fprintf(stderr, "Failed to create program from shaders %s and %s\n",
            vertex_shader_path, fragment_shader_path);
    exit(1);
  }

  return shader;
}

Shader load_shader_embedded(const uint8_t* vertex_shader,
                            const uint32_t vs_size, uint8_t* fragment_shader,
                            const uint32_t fs_size) {
  const bgfx_memory_t* vertex_mem = bgfx_make_ref(vertex_shader, vs_size);
  const bgfx_memory_t* fragment_mem = bgfx_make_ref(fragment_shader, fs_size);

  bgfx_shader_handle_t vertex_handle = bgfx_create_shader(vertex_mem);
  bgfx_shader_handle_t fragment_handle = bgfx_create_shader(fragment_mem);

  if (!BGFX_HANDLE_IS_VALID(vertex_handle) ||
      !BGFX_HANDLE_IS_VALID(fragment_handle)) {
    fprintf(stderr, "Failed to create shaders from embedded data\n");
    exit(1);
  }

  Shader shader;
  shader.vertex_shader = vertex_handle;
  shader.fragment_shader = fragment_handle;
  shader.program =
      bgfx_create_program(shader.vertex_shader, shader.fragment_shader, true);

  if (!BGFX_HANDLE_IS_VALID(shader.program)) {
    fprintf(stderr, "Failed to create program from embedded shaders\n");
    exit(1);
  }

  return shader;
}

void destroy_shader_program(Shader* shader) {
  bgfx_destroy_program(shader->program);
  bgfx_destroy_shader(shader->vertex_shader);
  bgfx_destroy_shader(shader->fragment_shader);
}

SDL_Surface* load_image(const char* filename) {
  SDL_Surface* surface = IMG_Load(filename);
  if (!surface) {
    fprintf(stderr, "Failed to load image %s: %s\n", filename, IMG_GetError());
    return NULL;
  }
  return surface;
}

bgfx_texture_handle_t create_texture_from_surface(SDL_Surface* surface) {
  if (!surface) {
    fprintf(stderr, "Cannot create texture from NULL surface\n");
    exit(1);
  }

  bgfx_texture_format_t format =
      BGFX_TEXTURE_FORMAT_BGRA8;
  const bgfx_memory_t* mem = bgfx_alloc(surface->pitch * surface->h);
  memcpy(mem->data, surface->pixels, surface->pitch * surface->h);

  bgfx_texture_handle_t handle =
      bgfx_create_texture_2d((uint16_t)surface->w, (uint16_t)surface->h,
                             false,
                             1, format, 0, mem);

  return handle;
}
