#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/c99/bgfx.h>
#include <cglm/cglm.h>
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#include <fs_transformations.sc.glsl.bin.h>
#include <vs_transformations.sc.glsl.bin.h>
#elif BX_PLATFORM_OSX
#include <fs_transformations.sc.mtl.bin.h>
#include <vs_transformations.sc.mtl.bin.h>
#elif BX_PLATFORM_WINDOWS
#include <fs_transformations.sc.dx11.bin.h>
#include <vs_transformations.sc.dx11.bin.h>
#endif
#include <setup_metal_layer.h>
#include <stdbool.h>
#include <stdio.h>
#include <utils.h>

#define BGFX_STATE_DEFAULT_NO_CULL                                      \
  (0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | \
   BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA)

typedef struct PosColorVertex {
  float x, y, z;
  uint32_t abgr;
  float u, v;
} PosColorVertex;

static PosColorVertex s_vertices[4] = {
    {0.5f, 0.5f, 0.0f, 0xff0000ff, 1.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 0xff00ffff, 1.0f, 0.0f},
    {-0.5f, -0.5f, 0.0f, 0xff00ff00, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.0f, 0xffffc300, 0.0f, 1.0f}};

static const uint16_t s_indices[6] = {0, 1, 3, 1, 2, 3};

bgfx_vertex_layout_t layout;
bgfx_vertex_buffer_handle_t vbh;
bgfx_index_buffer_handle_t ibh;
Shader shader_program;
bgfx_uniform_handle_t u_color;
bgfx_uniform_handle_t u_transformation;

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

  const bgfx_memory_t* index_mem = bgfx_copy(s_indices, sizeof(s_indices));
  ibh = bgfx_create_index_buffer(index_mem, BGFX_BUFFER_NONE);

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  shader_program = load_shader_embedded(vs_transformations_glsl, sizeof(vs_transformations_glsl), fs_transformations_glsl, sizeof(fs_transformations_glsl));
#elif BX_PLATFORM_OSX
  shader_program = load_shader_embedded(vs_transformations_mtl, sizeof(vs_transformations_mtl), fs_transformations_mtl, sizeof(fs_transformations_mtl));
#elif BX_PLATFORM_WINDOWS
  shader_program = load_shader_embedded(vs_transformations_dx11, sizeof(vs_transformations_dx11), fs_transformations_dx11, sizeof(fs_transformations_dx11));
#endif

  // Set uniforms
  u_color = bgfx_create_uniform("u_color", BGFX_UNIFORM_TYPE_VEC4, 1);
  u_transformation =
      bgfx_create_uniform("u_transformation", BGFX_UNIFORM_TYPE_MAT4, 1);
}

int main(int argc, char* argv[]) {
  uint32_t width = 800;
  uint32_t height = 600;
  uint32_t debug = BGFX_DEBUG_TEXT;
  uint32_t reset = BGFX_RESET_VSYNC;

  bgfx_init_t init;
  bgfx_init_ctor(&init);
  SDL_Window* window = init_sdl_bgfx(width, height, &init, debug, reset);
  SDL_SetWindowTitle(window, "Shaders Transformations");

  SDL_Surface* container_surface = load_image("../../../resources/container.jpg");
  bgfx_texture_handle_t container_texture =
      create_texture_from_surface(container_surface);
  SDL_FreeSurface(container_surface);

  SDL_Surface* awesomeface_surface = load_image("../../../resources/awesomeface.png");
  bgfx_texture_handle_t awesomeface_texture =
      create_texture_from_surface(awesomeface_surface);
  SDL_FreeSurface(awesomeface_surface);

  bgfx_uniform_handle_t s_texture_1 =
      bgfx_create_uniform("s_texture_1", BGFX_UNIFORM_TYPE_SAMPLER, 1);
  bgfx_uniform_handle_t s_texture_2 =
      bgfx_create_uniform("s_texture_2", BGFX_UNIFORM_TYPE_SAMPLER, 1);

  setup_buffers_and_shaders();

  bool running = true;
  float rotationAngle = 0.0f;
  float scaleFactor = 1.0f;

  // Inside your main loop, after setting up the initial container
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      process_input(&event, &running, &width, &height, reset, init);
    }

    bgfx_set_view_rect(0, 0, 0, width, height);
    bgfx_touch(0);

    bgfx_dbg_text_printf(0, 0, 0x4f, "Hello, Transformations!");

    // Uniform color setting for first container
    float greenValue = sin(SDL_GetTicks() * 0.001f);
    float color[4] = {0.0f, greenValue, 0.1f, 1.0f};
    bgfx_set_uniform(u_color, color, 1);

    // First container transformations
    mat4 transform1;
    glm_mat4_identity(transform1);

    vec3 translation1 = {0.5f, -0.5f, 0.0f};
    rotationAngle += 0.01f;
    scaleFactor = 1.0f + 0.2f * sin(SDL_GetTicks() * 0.001f);
    vec3 rotationAxis1 = {0.0f, 0.0f, 1.0f};

    glm_translate(transform1, translation1);
    glm_scale(transform1, (vec3){scaleFactor, scaleFactor, scaleFactor});
    glm_rotate(transform1, rotationAngle, rotationAxis1);

    bgfx_set_uniform(u_transformation, &transform1, 1);

    // Setting vertex and index buffers
    bgfx_set_vertex_buffer(0, vbh, 0, 4);
    bgfx_set_index_buffer(ibh, 0, 6);

    // Setting textures
    bgfx_set_texture(0, s_texture_1, container_texture, UINT32_MAX);
    bgfx_set_texture(1, s_texture_2, awesomeface_texture, UINT32_MAX);

    // Set render states
    bgfx_set_state(BGFX_STATE_DEFAULT_NO_CULL, 0);

    // Submit first container
    bgfx_submit(0, shader_program.program, 0, BGFX_DISCARD_ALL);

    // Second container transformations
    mat4 transform2;
    glm_mat4_identity(transform2);

    vec3 translation2 = {-0.5f, 0.5f, 0.0f};
    float scaleFactor2 = fabs(sin(SDL_GetTicks() * 0.001f));

    glm_translate(transform2, translation2);
    glm_scale(transform2, (vec3){scaleFactor2, scaleFactor2, scaleFactor2});

    bgfx_set_uniform(u_transformation, &transform2, 1);

    // Setting vertex and index buffers
    bgfx_set_vertex_buffer(0, vbh, 0, 4);
    bgfx_set_index_buffer(ibh, 0, 6);

    // Setting textures
    bgfx_set_texture(0, s_texture_1, container_texture, UINT32_MAX);
    bgfx_set_texture(1, s_texture_2, awesomeface_texture, UINT32_MAX);

    // Set render states
    bgfx_set_state(BGFX_STATE_DEFAULT_NO_CULL, 0);

    // Submit second container
    bgfx_submit(0, shader_program.program, 0, BGFX_DISCARD_ALL);

    bgfx_frame(false);
  }

  destroy_shader_program(&shader_program);
  bgfx_destroy_index_buffer(ibh);
  bgfx_destroy_vertex_buffer(vbh);
  bgfx_destroy_uniform(u_color);
  bgfx_destroy_uniform(u_transformation);
  bgfx_destroy_texture(container_texture);
  bgfx_destroy_texture(awesomeface_texture);
  bgfx_destroy_uniform(s_texture_1);
  bgfx_destroy_uniform(s_texture_2);

  bgfx_shutdown();

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
