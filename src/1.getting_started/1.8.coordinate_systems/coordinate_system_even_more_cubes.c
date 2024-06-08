#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/c99/bgfx.h>
#include <cglm/cglm.h>
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#include <fs_coordinate_system.sc.glsl.bin.h>
#include <vs_coordinate_system.sc.glsl.bin.h>
#elif BX_PLATFORM_OSX
#include <fs_coordinate_system.sc.mtl.bin.h>
#include <vs_coordinate_system.sc.mtl.bin.h>
#elif BX_PLATFORM_WINDOWS
#include <fs_coordinate_system.sc.dx11.bin.h>
#include <vs_coordinate_system.sc.dx11.bin.h>
#endif
#include <setup_metal_layer.h>
#include <stdbool.h>
#include <stdio.h>
#include <utils.h>

#define BGFX_STATE_DEFAULT_NO_CULL                                      \
  (0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | \
   BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA)

#define NUM_CUBES 100

typedef struct Cube {
  vec3 position;
} Cube;

Cube cubes[NUM_CUBES];

typedef struct PosColorVertex {
  float x, y, z;
  uint32_t abgr;
  float u, v;
} PosColorVertex;

static PosColorVertex s_vertices[36] = {
    // Front face
    {-0.5f, -0.5f, -0.5f, 0xff0000ff, 0.0f, 0.0f},  // red
    {0.5f, -0.5f, -0.5f, 0xff00ffff, 1.0f, 0.0f},   // cyan
    {0.5f, 0.5f, -0.5f, 0xff00ff00, 1.0f, 1.0f},    // green
    {0.5f, 0.5f, -0.5f, 0xff00ff00, 1.0f, 1.0f},    // green
    {-0.5f, 0.5f, -0.5f, 0xffffc300, 0.0f, 1.0f},   // yellow
    {-0.5f, -0.5f, -0.5f, 0xff0000ff, 0.0f, 0.0f},  // red

    // Back face
    {-0.5f, -0.5f, 0.5f, 0xffff0000, 0.0f, 0.0f},  // blue
    {0.5f, -0.5f, 0.5f, 0xff00ffff, 1.0f, 0.0f},   // cyan
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 1.0f},    // green
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 1.0f},    // green
    {-0.5f, 0.5f, 0.5f, 0xffffc300, 0.0f, 1.0f},   // yellow
    {-0.5f, -0.5f, 0.5f, 0xffff0000, 0.0f, 0.0f},  // blue

    // Left face
    {-0.5f, 0.5f, 0.5f, 0xffff00ff, 1.0f, 0.0f},    // magenta
    {-0.5f, 0.5f, -0.5f, 0xff00ff00, 1.0f, 1.0f},   // green
    {-0.5f, -0.5f, -0.5f, 0xff0000ff, 0.0f, 1.0f},  // red
    {-0.5f, -0.5f, -0.5f, 0xff0000ff, 0.0f, 1.0f},  // red
    {-0.5f, -0.5f, 0.5f, 0xffff0000, 0.0f, 0.0f},   // blue
    {-0.5f, 0.5f, 0.5f, 0xffff00ff, 1.0f, 0.0f},    // magenta

    // Right face
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 0.0f},    // green
    {0.5f, 0.5f, -0.5f, 0xff0000ff, 1.0f, 1.0f},   // red
    {0.5f, -0.5f, -0.5f, 0xff00ffff, 0.0f, 1.0f},  // cyan
    {0.5f, -0.5f, -0.5f, 0xff00ffff, 0.0f, 1.0f},  // cyan
    {0.5f, -0.5f, 0.5f, 0xffffc300, 0.0f, 0.0f},   // yellow
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 0.0f},    // green

    // Bottom face
    {-0.5f, -0.5f, -0.5f, 0xffffc300, 0.0f, 1.0f},  // yellow
    {0.5f, -0.5f, -0.5f, 0xff00ffff, 1.0f, 1.0f},   // cyan
    {0.5f, -0.5f, 0.5f, 0xffff00ff, 1.0f, 0.0f},    // magenta
    {0.5f, -0.5f, 0.5f, 0xffff00ff, 1.0f, 0.0f},    // magenta
    {-0.5f, -0.5f, 0.5f, 0xffff0000, 0.0f, 0.0f},   // blue
    {-0.5f, -0.5f, -0.5f, 0xffffc300, 0.0f, 1.0f},  // yellow

    // Top face
    {-0.5f, 0.5f, -0.5f, 0xffff00ff, 0.0f, 1.0f},  // magenta
    {0.5f, 0.5f, -0.5f, 0xff00ffff, 1.0f, 1.0f},   // cyan
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 0.0f},    // green
    {0.5f, 0.5f, 0.5f, 0xff00ff00, 1.0f, 0.0f},    // green
    {-0.5f, 0.5f, 0.5f, 0xffffc300, 0.0f, 0.0f},   // yellow
    {-0.5f, 0.5f, -0.5f, 0xffff00ff, 0.0f, 1.0f}   // magenta
};

bgfx_vertex_layout_t layout;
bgfx_vertex_buffer_handle_t vbh;
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

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  shader_program = load_shader_embedded(vs_coordinate_system_glsl, sizeof(vs_coordinate_system_glsl), fs_coordinate_system_glsl, sizeof(fs_coordinate_system_glsl));
#elif BX_PLATFORM_OSX
  shader_program = load_shader_embedded(vs_coordinate_system_mtl, sizeof(vs_coordinate_system_mtl), fs_coordinate_system_mtl, sizeof(fs_coordinate_system_mtl));
#elif BX_PLATFORM_WINDOWS
  shader_program = load_shader_embedded(vs_coordinate_system_dx11, sizeof(vs_coordinate_system_dx11), fs_coordinate_system_dx11, sizeof(fs_coordinate_system_dx11));
#endif

  // Set uniforms
  u_color = bgfx_create_uniform("u_color", BGFX_UNIFORM_TYPE_VEC4, 1);
}

void generate_random_positions() {
  for (int i = 0; i < NUM_CUBES; i++) {
    cubes[i].position[0] = (rand() % 200 - 100) / 10.0f;  // X position
    cubes[i].position[1] = (rand() % 200 - 100) / 10.0f;  // Y position
    cubes[i].position[2] = (rand() % 200 - 100) / 10.0f;  // Z position

    printf("Cube %d position: %f, %f, %f\n", i, cubes[i].position[0],
           cubes[i].position[1], cubes[i].position[2]);
  }
}

int main(int argc, char* argv[]) {
  uint32_t width = 800;
  uint32_t height = 600;
  uint32_t debug = BGFX_DEBUG_TEXT;  // set to BGFX_DEBUG_TEXT |
                                     // BGFX_DEBUG_WIREFRAME to see wireframe
  uint32_t reset = BGFX_RESET_VSYNC;

  generate_random_positions();

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
    glm_translate(view,
                  (vec3){0.0f, 0.0f, -15.0f});  // move the camera backwards

    mat4 proj;
    glm_perspective(glm_rad(45.0f), (float)width / (float)height, 0.1f, 100.0f,
                    proj);

    bgfx_set_view_transform(0, view, proj);

    for (int i = 0; i < NUM_CUBES; i++) {
      mat4 model;
      glm_mat4_identity(model);
      glm_translate(model, cubes[i].position);
      glm_rotate(model, glm_rad((float)SDL_GetTicks() * 0.05f),
                 (vec3){1.0f, 0.5f, 0.0f});

      bgfx_set_transform(model, 1);

      bgfx_set_vertex_buffer(0, vbh, 0, 36);

      bgfx_set_texture(0, s_texture_1, container_texture, UINT32_MAX);
      bgfx_set_texture(1, s_texture_2, awesomeface_texture, UINT32_MAX);

      bgfx_set_state(BGFX_STATE_DEFAULT_NO_CULL, 0);

      bgfx_submit(0, shader_program.program, 0, BGFX_DISCARD_ALL);
    }

    bgfx_frame(false);
  }

  destroy_shader_program(&shader_program);
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
