#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/c99/bgfx.h>
#include <cglm/cglm.h>
#include <fs_coordinate_system.sc.mtl.bin.h>
#include <setup_metal_layer.h>
#include <stdbool.h>
#include <stdio.h>
#include <utils.h>
#include <vs_coordinate_system.sc.mtl.bin.h>

#define BGFX_STATE_DEFAULT_NO_CULL                                      \
  (0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | \
   BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA)

typedef struct PosColorVertex {
  float x, y, z;
  uint32_t abgr;
  float u, v;
} PosColorVertex;

// Updated vertex data to match LearnOpenGL conventions
// static PosColorVertex s_vertices[4] = {
//     {-0.5f, -0.5f, 0.0f, 0xff0000ff, 0.0f, 1.0f},
//     {0.5f, -0.5f, 0.0f, 0xff00ffff, 1.0f, 1.0f},
//     {0.5f, 0.5f, 0.0f, 0xff00ff00, 1.0f, 0.0f},
//     {-0.5f, 0.5f, 0.0f, 0xffffc300, 0.0f, 0.0f}};
static PosColorVertex s_vertices[36] = {
    // Front face
    {-0.5f, -0.5f, -0.5f, 0xff0000ff, 0.0f, 0.0f}, // red
    {0.5f, -0.5f, -0.5f, 0xff00ffff, 1.0f, 0.0f},  // cyan
    {0.5f, 0.5f, -0.5f, 0xff00ff00, 1.0f, 1.0f},   // green
    {0.5f, 0.5f, -0.5f, 0xff00ff00, 1.0f, 1.0f},   // green
    {-0.5f, 0.5f, -0.5f, 0xffffc300, 0.0f, 1.0f},  // yellow
    {-0.5f, -0.5f, -0.5f, 0xff0000ff, 0.0f, 0.0f}, // red

    // Back face
    {-0.5f, -0.5f, 0.5f, 0xffff0000, 0.0f, 0.0f},  // blue
    {0.5f, -0.5f, 0.5f, 0xff00ffff, 1.0f, 0.0f},   // cyan
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 1.0f},    // green
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 1.0f},    // green
    {-0.5f, 0.5f, 0.5f, 0xffffc300, 0.0f, 1.0f},   // yellow
    {-0.5f, -0.5f, 0.5f, 0xffff0000, 0.0f, 0.0f},  // blue

    // Left face
    {-0.5f, 0.5f, 0.5f, 0xffff00ff, 1.0f, 0.0f},   // magenta
    {-0.5f, 0.5f, -0.5f, 0xff00ff00, 1.0f, 1.0f},  // green
    {-0.5f, -0.5f, -0.5f, 0xff0000ff, 0.0f, 1.0f}, // red
    {-0.5f, -0.5f, -0.5f, 0xff0000ff, 0.0f, 1.0f}, // red
    {-0.5f, -0.5f, 0.5f, 0xffff0000, 0.0f, 0.0f},  // blue
    {-0.5f, 0.5f, 0.5f, 0xffff00ff, 1.0f, 0.0f},   // magenta

    // Right face
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 0.0f},    // green
    {0.5f, 0.5f, -0.5f, 0xff0000ff, 1.0f, 1.0f},   // red
    {0.5f, -0.5f, -0.5f, 0xff00ffff, 0.0f, 1.0f},  // cyan
    {0.5f, -0.5f, -0.5f, 0xff00ffff, 0.0f, 1.0f},  // cyan
    {0.5f, -0.5f, 0.5f, 0xffffc300, 0.0f, 0.0f},   // yellow
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 0.0f},    // green

    // Bottom face
    {-0.5f, -0.5f, -0.5f, 0xffffc300, 0.0f, 1.0f}, // yellow
    {0.5f, -0.5f, -0.5f, 0xff00ffff, 1.0f, 1.0f},  // cyan
    {0.5f, -0.5f, 0.5f, 0xffff00ff, 1.0f, 0.0f},   // magenta
    {0.5f, -0.5f, 0.5f, 0xffff00ff, 1.0f, 0.0f},   // magenta
    {-0.5f, -0.5f, 0.5f, 0xffff0000, 0.0f, 0.0f},  // blue
    {-0.5f, -0.5f, -0.5f, 0xffffc300, 0.0f, 1.0f}, // yellow

    // Top face
    {-0.5f, 0.5f, -0.5f, 0xffff00ff, 0.0f, 1.0f},  // magenta
    {0.5f, 0.5f, -0.5f, 0xff00ffff, 1.0f, 1.0f},   // cyan
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 0.0f},    // green
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 0.0f},    // green
    {-0.5f, 0.5f, 0.5f, 0xffffc300, 0.0f, 0.0f},   // yellow
    {-0.5f, 0.5f, -0.5f, 0xffff00ff, 0.0f, 1.0f}   // magenta
};


// static const uint16_t s_indices[6] = {0, 1, 2, 0, 2, 3};

bgfx_vertex_layout_t layout;
bgfx_vertex_buffer_handle_t vbh;
// bgfx_index_buffer_handle_t ibh;
Shader shader_program;
bgfx_uniform_handle_t u_color;

void setup_buffers_and_shaders() {
  bgfx_vertex_layout_begin(&layout, bgfx_get_renderer_type());
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_POSITION, 3,
                         BGFX_ATTRIB_TYPE_FLOAT, false, false);
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8,
                         true, false);
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_TEXCOORD0, 2,
                         BGFX_ATTRIB_TYPE_FLOAT, false, false);
  bgfx_vertex_layout_end(&layout);

  const bgfx_memory_t* vertex_mem = bgfx_copy(s_vertices, sizeof(s_vertices));
  vbh = bgfx_create_vertex_buffer(vertex_mem, &layout, BGFX_BUFFER_NONE);

  // const bgfx_memory_t* index_mem = bgfx_copy(s_indices, sizeof(s_indices));
  // ibh = bgfx_create_index_buffer(index_mem, BGFX_BUFFER_NONE);

  shader_program = load_shader_embedded(
      (uint8_t*)vs_coordinate_system_mtl, sizeof(vs_coordinate_system_mtl),
      (uint8_t*)fs_coordinate_system_mtl, sizeof(fs_coordinate_system_mtl));

  // Set uniforms
  u_color = bgfx_create_uniform("u_color", BGFX_UNIFORM_TYPE_VEC4, 1);
}

int main(int argc, char* argv[]) {
  uint32_t width = 800;
  uint32_t height = 600;
  uint32_t debug = BGFX_DEBUG_TEXT;  // set to BGFX_DEBUG_TEXT |
                                     // BGFX_DEBUG_WIREFRAME to see wireframe
  uint32_t reset = BGFX_RESET_VSYNC;

  bgfx_init_t init;
  bgfx_init_ctor(&init);
  SDL_Window* window = init_sdl_bgfx(width, height, &init, debug, reset);
  SDL_SetWindowTitle(window, "Shaders Coordinates Systems");

  SDL_Surface* container_surface = load_image("container.jpg");
  bgfx_texture_handle_t container_texture =
      create_texture_from_surface(container_surface);
  SDL_FreeSurface(container_surface);

  SDL_Surface* awesomeface_surface = load_image("awesomeface.png");
  bgfx_texture_handle_t awesomeface_texture =
      create_texture_from_surface(awesomeface_surface);
  SDL_FreeSurface(awesomeface_surface);

  bgfx_uniform_handle_t s_texture_1 =
      bgfx_create_uniform("s_texture_1", BGFX_UNIFORM_TYPE_SAMPLER, 1);
  bgfx_uniform_handle_t s_texture_2 =
      bgfx_create_uniform("s_texture_2", BGFX_UNIFORM_TYPE_SAMPLER, 1);

  setup_buffers_and_shaders();

  bool running = true;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      process_input(&event, &running, &width, &height, reset, init);
    }

    bgfx_set_view_rect(0, 0, 0, width, height);
    bgfx_touch(0);

    bgfx_dbg_text_printf(0, 0, 0x4f, "Hello, Coordinates Systems!");

    float greenValue = sin(SDL_GetTicks() * 0.001f);
    float color[4] = {0.0f, greenValue, 0.1f, 1.0f};
    bgfx_set_uniform(u_color, color, 1);

    mat4 view;
    glm_mat4_identity(view);
    glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

    mat4 proj;
    glm_perspective(glm_rad(45.0f), (float)width / (float)height, 0.1f, 100.0f,
                    proj);

    bgfx_set_view_transform(0, view, proj);

    mat4 model;
    glm_mat4_identity(model);
    // glm_rotate(model, glm_rad(-55.0f), (vec3){1.0f, 0.0f, 0.0f});
    // rotate based on time
    glm_rotate(model, glm_rad((float)SDL_GetTicks() * 0.01f), (vec3){1.0f, 0.5f, 0.0f});

    bgfx_set_transform(model, 1);

    bgfx_set_vertex_buffer(0, vbh, 0, 36);
    // bgfx_set_index_buffer(ibh, 0, 6);

    bgfx_set_texture(0, s_texture_1, container_texture, UINT32_MAX);
    bgfx_set_texture(1, s_texture_2, awesomeface_texture, UINT32_MAX);


    // Use CCW culling to match LearnOpenGL data
    bgfx_set_state(BGFX_STATE_DEFAULT_NO_CULL, 0);

    bgfx_submit(0, shader_program.program, 0, BGFX_DISCARD_ALL);

    bgfx_frame(false);
  }

  destroy_shader_program(&shader_program);
  // bgfx_destroy_index_buffer(ibh);
  bgfx_destroy_vertex_buffer(vbh);
  bgfx_destroy_texture(container_texture);
  bgfx_destroy_texture(awesomeface_texture);
  bgfx_destroy_uniform(u_color);
  bgfx_destroy_uniform(s_texture_1);
  bgfx_destroy_uniform(s_texture_2);

  bgfx_shutdown();

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
