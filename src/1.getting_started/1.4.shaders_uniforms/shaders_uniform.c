#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/c99/bgfx.h>
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#include <fs_uniform.sc.glsl.bin.h>
#include <vs_uniform.sc.glsl.bin.h>
#elif BX_PLATFORM_OSX
#include <fs_uniform.sc.mtl.bin.h>
#include <vs_uniform.sc.mtl.bin.h>
#elif BX_PLATFORM_WINDOWS
#include <fs_uniform.sc.dx11.bin.h>
#include <vs_uniform.sc.dx11.bin.h>
#endif
#include <setup_metal_layer.h>
#include <stdbool.h>
#include <stdio.h>
#include <utils.h>

typedef struct PosColorVertex {
  float x, y, z;
  uint32_t abgr;
} PosColorVertex;

static PosColorVertex s_vertices[4] = {{-0.5f, -0.5f, 0.0f, 0xff0000ff},
                                       {0.5f, -0.5f, 0.0f, 0xff00ffff},
                                       {0.5f, 0.5f, 0.0f, 0xff00ff00},
                                       {-0.5f, 0.5f, 0.0f, 0xffffc300}};

static const uint16_t s_indices[6] = {0, 1, 2, 0, 2, 3};

bgfx_vertex_layout_t layout;
bgfx_vertex_buffer_handle_t vbh;
bgfx_index_buffer_handle_t ibh;
bgfx_program_handle_t program;
bgfx_uniform_handle_t u_color;

void setup_buffers_and_shaders() {
  bgfx_vertex_layout_begin(&layout, bgfx_get_renderer_type());
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_POSITION, 3,
                         BGFX_ATTRIB_TYPE_FLOAT, false, false);
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8,
                         true, false);
  bgfx_vertex_layout_end(&layout);

  const bgfx_memory_t* vertex_mem = bgfx_copy(s_vertices, sizeof(s_vertices));
  vbh = bgfx_create_vertex_buffer(vertex_mem, &layout, BGFX_BUFFER_NONE);

  const bgfx_memory_t* index_mem = bgfx_copy(s_indices, sizeof(s_indices));
  ibh = bgfx_create_index_buffer(index_mem, BGFX_BUFFER_NONE);

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  bgfx_shader_handle_t vsh = bgfx_create_shader(
      bgfx_make_ref(vs_uniform_glsl, sizeof(vs_uniform_glsl)));
  bgfx_shader_handle_t fsh = bgfx_create_shader(
      bgfx_make_ref(fs_uniform_glsl, sizeof(fs_uniform_glsl)));
#elif BX_PLATFORM_OSX
  bgfx_shader_handle_t vsh = bgfx_create_shader(
      bgfx_make_ref(vs_uniform_mtl, sizeof(vs_uniform_mtl)));
  bgfx_shader_handle_t fsh = bgfx_create_shader(
      bgfx_make_ref(fs_uniform_mtl, sizeof(fs_uniform_mtl)));
#elif BX_PLATFORM_WINDOWS
  bgfx_shader_handle_t vsh = bgfx_create_shader(
      bgfx_make_ref(vs_uniform_dx11, sizeof(vs_uniform_dx11)));
  bgfx_shader_handle_t fsh = bgfx_create_shader(
      bgfx_make_ref(fs_uniform_dx11, sizeof(fs_uniform_dx11)));
#endif

  program = bgfx_create_program(vsh, fsh, true);

  // Set uniforms
  u_color = bgfx_create_uniform("u_color", BGFX_UNIFORM_TYPE_VEC4, 1);
}

int main(int argc, char* argv[]) {
  uint32_t width = 800;
  uint32_t height = 600;
  uint32_t debug = BGFX_DEBUG_TEXT; // set to BGFX_DEBUG_TEXT | BGFX_DEBUG_WIREFRAME to see wireframe
  uint32_t reset = BGFX_RESET_VSYNC;

  bgfx_init_t init;
  bgfx_init_ctor(&init);
  SDL_Window* window = init_sdl_bgfx(width, height, &init, debug, reset);
  SDL_SetWindowTitle(window, "Shaders Uniform");

  setup_buffers_and_shaders();

  bool running = true;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      process_input(&event, &running, &width, &height, reset, init);
    }

    bgfx_set_view_rect(0, 0, 0, width, height);
    bgfx_touch(0);

    bgfx_dbg_text_printf(0, 0, 0x4f, "Hello, Shaders Uniform!");

    float greenValue = sin(SDL_GetTicks() * 0.001f);
    float color[4] = {0.0f, greenValue, 0.1f, 1.0f};
    bgfx_set_uniform(u_color, color, 1);

    bgfx_set_vertex_buffer(0, vbh, 0, 4);
    bgfx_set_index_buffer(ibh, 0, 6);

    bgfx_submit(0, program, 0, BGFX_DISCARD_ALL);

    bgfx_frame(false);
  }

  bgfx_destroy_program(program);
  bgfx_destroy_index_buffer(ibh);
  bgfx_destroy_vertex_buffer(vbh);
  bgfx_destroy_uniform(u_color);
  bgfx_shutdown();

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
