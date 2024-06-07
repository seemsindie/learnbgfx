#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/c99/bgfx.h>
#include <fs_triangle.sc.mtl.bin.h>
#include <setup_metal_layer.h>
#include <stdbool.h>
#include <stdio.h>
#include <vs_triangle.sc.mtl.bin.h>
#include <utils.h>

typedef struct PosColorVertex {
  float x, y, z;
  uint32_t abgr;
} PosColorVertex;

static PosColorVertex s_vertices[3] = {{-0.5f, -0.5f, 0.0f, 0xff0000ff},
                                       {0.5f, -0.5f, 0.0f, 0xff00ffff},
                                       {0.0f, 0.5f, 0.0f, 0xffffc300}};

bgfx_vertex_layout_t layout;
bgfx_vertex_buffer_handle_t vbh;
bgfx_program_handle_t program;

// Setup vertex buffer, and load shaders
void setup_buffers_and_shaders() {
  bgfx_vertex_layout_begin(&layout, bgfx_get_renderer_type());
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_POSITION, 3,
                         BGFX_ATTRIB_TYPE_FLOAT, false, false);
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8,
                         true, false);
  bgfx_vertex_layout_end(&layout);

  const bgfx_memory_t* vertex_mem = bgfx_copy(s_vertices, sizeof(s_vertices));
  vbh = bgfx_create_vertex_buffer(vertex_mem, &layout, BGFX_BUFFER_NONE);

  bgfx_shader_handle_t vsh = bgfx_create_shader(
      bgfx_make_ref(vs_triangle_mtl, sizeof(vs_triangle_mtl)));
  bgfx_shader_handle_t fsh = bgfx_create_shader(
      bgfx_make_ref(fs_triangle_mtl, sizeof(fs_triangle_mtl)));

  program = bgfx_create_program(vsh, fsh, true);
}

int main(int argc, char* argv[]) {
  uint32_t width = 800;
  uint32_t height = 600;
  uint32_t debug = BGFX_DEBUG_TEXT;
  uint32_t reset = BGFX_RESET_VSYNC;

  bgfx_init_t init;
  bgfx_init_ctor(&init);
  SDL_Window* window = init_sdl_bgfx(width, height, &init, debug, reset);
  SDL_SetWindowTitle(window, "Triangle");

  setup_buffers_and_shaders();

  bool running = true;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      process_input(&event, &running, &width, &height, reset, init);
    }

    bgfx_set_view_rect(0, 0, 0, width, height);
    bgfx_touch(0);
    bgfx_dbg_text_printf(0, 0, 0x4f, "Hello, World!");

    bgfx_set_vertex_buffer(0, vbh, 0, 3);
    bgfx_submit(0, program, 0, BGFX_DISCARD_ALL);

    bgfx_frame(false);
  }

  bgfx_destroy_program(program);
  bgfx_destroy_vertex_buffer(vbh);
  bgfx_shutdown();

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}