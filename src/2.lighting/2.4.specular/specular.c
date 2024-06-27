#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/c99/bgfx.h>
#include <cglm/cglm.h>
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#include <fs_specular_light.sc.glsl.bin.h>
#include <fs_specular.sc.glsl.bin.h>
#include <vs_specular_light.sc.glsl.bin.h>
#include <vs_specular.sc.glsl.bin.h>
#elif BX_PLATFORM_OSX
#include <fs_specular_light.sc.mtl.bin.h>
#include <fs_specular.sc.mtl.bin.h>
#include <vs_specular_light.sc.mtl.bin.h>
#include <vs_specular.sc.mtl.bin.h>
#elif BX_PLATFORM_WINDOWS
#include <fs_specular_light.sc.dx11.bin.h>
#include <fs_specular.sc.dx11.bin.h>
#include <vs_specular_light.sc.dx11.bin.h>
#include <vs_specular.sc.dx11.bin.h>
#endif
#include <camera.h>
#include <setup_metal_layer.h>
#include <stdbool.h>
#include <stdio.h>
#include <utils.h>

#define BGFX_STATE_DEFAULT_NO_CULL                                      \
  (0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | \
   BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA)

typedef struct Cube {
  vec3 position;
} Cube;

typedef struct PosColorVertex {
  float x, y, z;
  float nx, ny, nz;
} PosColorVertex;

static PosColorVertex s_vertices[36] = {
    // Front face (normal: 0, 0, -1)
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},

    // Back face (normal: 0, 0, 1)
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},

    // Left face (normal: -1, 0, 0)
    {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f},

    // Right face (normal: 1, 0, 0)
    {0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f},

    // Bottom face (normal: 0, -1, 0)
    {-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f},
    {0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f},
    {0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f},
    {0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f},

    // Top face (normal: 0, 1, 0)
    {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
};


bgfx_vertex_layout_t layout;
bgfx_vertex_buffer_handle_t vbh;
Shader light_program;
Shader shader_program;
bgfx_uniform_handle_t u_objectColor;
bgfx_uniform_handle_t u_lightColor;
bgfx_uniform_handle_t u_lightPos;
bgfx_uniform_handle_t u_viewPos;
bgfx_uniform_handle_t u_normalMatrix;

// Create Camera struct
Camera camera;

// Function to setup buffers and shaders
void setup_buffers_and_shaders() {
  bgfx_vertex_layout_begin(&layout, bgfx_get_renderer_type());
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_POSITION, 3,
                         BGFX_ATTRIB_TYPE_FLOAT, false, false);
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_NORMAL, 3,
                         BGFX_ATTRIB_TYPE_FLOAT, false, false);
  bgfx_vertex_layout_end(&layout);

  const bgfx_memory_t* vertex_mem = bgfx_copy(s_vertices, sizeof(s_vertices));
  vbh = bgfx_create_vertex_buffer(vertex_mem, &layout, BGFX_BUFFER_NONE);

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  shader_program =
      load_shader_embedded(vs_specular_glsl, sizeof(vs_specular_glsl),
                           fs_specular_glsl, sizeof(fs_specular_glsl));
  light_program = load_shader_embedded(vs_specular_light_glsl, sizeof(vs_specular_light_glsl),
                                       fs_specular_light_glsl, sizeof(fs_specular_light_glsl));
#elif BX_PLATFORM_OSX
  shader_program =
      load_shader_embedded(vs_specular_mtl, sizeof(vs_specular_mtl),
                           fs_specular_mtl, sizeof(fs_specular_mtl));
  light_program = load_shader_embedded(vs_specular_light_mtl, sizeof(vs_specular_light_mtl),
                                       fs_specular_light_mtl, sizeof(fs_specular_light_mtl));
#elif BX_PLATFORM_WINDOWS
  shader_program =
      load_shader_embedded(vs_specular_dx11, sizeof(vs_specular_dx11),
                           fs_specular_dx11, sizeof(fs_specular_dx11));
  light_program = load_shader_embedded(vs_specular_light_dx11, sizeof(vs_specular_light_dx11),
                                       fs_specular_light_dx11, sizeof(fs_specular_light_dx11));
#endif

  // Set uniforms
  u_objectColor =
      bgfx_create_uniform("u_objectColor", BGFX_UNIFORM_TYPE_VEC4, 1);
  u_lightColor = bgfx_create_uniform("u_lightColor", BGFX_UNIFORM_TYPE_VEC4, 1);
  u_lightPos = bgfx_create_uniform("u_lightPos", BGFX_UNIFORM_TYPE_VEC4, 1);
  u_viewPos = bgfx_create_uniform("u_viewPos", BGFX_UNIFORM_TYPE_VEC4, 1);
  u_normalMatrix = bgfx_create_uniform("u_normalMatrix", BGFX_UNIFORM_TYPE_MAT3, 1);
}

void process_camera_movement(Camera* camera, const Uint8* key_state,
                             float deltaTime) {
  if (key_state[SDL_SCANCODE_W]) {
    camera_process_keyboard(camera, FORWARD, deltaTime);
  }
  if (key_state[SDL_SCANCODE_S]) {
    camera_process_keyboard(camera, BACKWARD, deltaTime);
  }
  if (key_state[SDL_SCANCODE_A]) {
    camera_process_keyboard(camera, LEFT, deltaTime);
  }
  if (key_state[SDL_SCANCODE_D]) {
    camera_process_keyboard(camera, RIGHT, deltaTime);
  }
}

void process_mouse_movement(Camera* camera) {
  int x, y;
  SDL_GetMouseState(&x, &y);

  static int lastX, lastY;
  static bool firstMouse = true;

  if (firstMouse) {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }

  int xOffset = x - lastX;
  int yOffset = lastY - y;

  lastX = x;
  lastY = y;

  camera_process_mouse_movement(camera, xOffset, yOffset, 1);
}

int main(int argc, char* argv[]) {
  uint32_t width = 800;
  uint32_t height = 600;
  uint32_t debug = BGFX_DEBUG_TEXT;   // set to BGFX_DEBUG_TEXT |
                                      // BGFX_DEBUG_WIREFRAME to see wireframe
  uint32_t reset = BGFX_RESET_VSYNC;  // BGFX_RESET_NONE

  // Initialize camera
  vec3 camera_position = {0.0f, 1.5f, 4.0f};
  vec3 camera_up = {0.0f, 1.0f, 0.0f};
  init_camera(&camera, camera_position, camera_up, YAW, -20.0f);

  // generate_random_positions();

  bgfx_init_t init;
  bgfx_init_ctor(&init);
  SDL_Window* window = init_sdl_bgfx(width, height, &init, debug, reset);
  SDL_SetWindowTitle(window, "Lighting Specular");

  SDL_Surface* container_surface =
      load_image("../../../resources/container.jpg");
  bgfx_texture_handle_t container_texture =
      create_texture_from_surface(container_surface);
  SDL_FreeSurface(container_surface);

  SDL_Surface* awesomeface_surface =
      load_image("../../../resources/awesomeface.png");
  bgfx_texture_handle_t awesomeface_texture =
      create_texture_from_surface(awesomeface_surface);
  SDL_FreeSurface(awesomeface_surface);

  bgfx_uniform_handle_t s_texture_1 =
      bgfx_create_uniform("s_texture_1", BGFX_UNIFORM_TYPE_SAMPLER, 1);
  bgfx_uniform_handle_t s_texture_2 =
      bgfx_create_uniform("s_texture_2", BGFX_UNIFORM_TYPE_SAMPLER, 1);

  setup_buffers_and_shaders();

  bool running = true;
  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;
  const float radius = 10.0f;

  while (running) {
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    deltaTime =
        (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) *
        0.001;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }

      // if left right key pressed rotate the camera with rotate_camera(camrera, angle)
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) {
          rotate_camera(&camera, -0.9f, false);
        }
        if (event.key.keysym.sym == SDLK_RIGHT) {
          rotate_camera(&camera, 0.9f, false);
        }
        if (event.key.keysym.sym == SDLK_UP) {
          rotate_camera(&camera, 0.9f, true);
        }
        if (event.key.keysym.sym == SDLK_DOWN) {
          rotate_camera(&camera, -0.9f, true);
        }
      }

      // mouse wheel zoom
      if (event.type == SDL_MOUSEWHEEL) {
        // camera_process_mouse_scroll(&camera, -event.wheel.y);
        camera_dolly_zoom(&camera, event.wheel.y * 0.1f);
      }
    }

    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    process_camera_movement(&camera, key_state, (float)deltaTime);

    // process_mouse_movement(&camera);

    bgfx_set_view_rect(0, 0, 0, width, height);
    bgfx_touch(0);

    bgfx_dbg_text_printf(0, 0, 0x4f, "Hello, Specular!");

    // Calculate camera position
    float camX = sin(SDL_GetTicks() / 10000.0f) * radius;
    float camZ = cos(SDL_GetTicks() / 10000.0f) * radius;
    mat4 view;
    camera_get_view_matrix(&camera, view);
    // glm_lookat((vec3){camX, 0.0f, camZ},  // Camera position
    //                (vec3){0.0f, 0.0f, 0.0f},  // Look at the origin
    //                (vec3){0.0f, 1.0f, 0.0f},  // Up vector
    //                view);

    mat4 proj;
    glm_perspective(glm_rad(45.0f), (float)width / (float)height, 0.1f, 100.0f,
                    proj);
    bgfx_set_view_transform(0, view, proj);

    // uniforms
    vec4 light_pos = {0.0f, 1.0f, -2.0f, 1.0f};
    bgfx_set_uniform(u_lightPos, light_pos, 1);
    bgfx_set_uniform(u_objectColor, (vec4){1.0f, 0.5f, 0.31f, 1.0f},
                     1);
    bgfx_set_uniform(u_lightColor, (vec4){1.0f, 1.0f, 1.0f, 1.0f}, 1);
    bgfx_set_uniform(u_viewPos, (vec4){camera.Position[0], camera.Position[1],
                                      camera.Position[2], 1.0f},
                     1);

    mat4 model, normalMatrix;
    glm_mat4_identity(model);
    // glm_rotate(model, glm_rad((float)SDL_GetTicks() * 0.05f),
              //  (vec3){1.0f, -0.5f, 1.0f});

    // glm_mat4_pick3(model, normalMatrix);
    // glm_mat4_inv(model, normalMatrix);
    // glm_mat4_transpose(normalMatrix);

    mat3 normalMatrix3;
    glm_mat4_pick3(model, normalMatrix3);
    glm_mat3_inv(normalMatrix3, normalMatrix3);
    glm_mat3_transpose(normalMatrix3);

    bgfx_set_uniform(u_normalMatrix, normalMatrix3, 1);
    
    bgfx_set_transform(model, 1);

    bgfx_set_vertex_buffer(0, vbh, 0, 36);

    bgfx_set_texture(0, s_texture_1, container_texture, UINT32_MAX);
    bgfx_set_texture(1, s_texture_2, awesomeface_texture, UINT32_MAX);

    bgfx_set_state(BGFX_STATE_DEFAULT_NO_CULL, 0);
    bgfx_submit(0, shader_program.program, 0, BGFX_DISCARD_ALL);

    // Draw light cube
    glm_mat4_identity(model);
    glm_translate(model, light_pos);
    glm_scale(model, (vec3){0.2f, 0.2f, 0.2f});
    bgfx_set_transform(model, 1);

    bgfx_set_vertex_buffer(0, vbh, 0, 36);

    bgfx_set_texture(0, s_texture_1, container_texture, UINT32_MAX);
    bgfx_set_texture(1, s_texture_2, awesomeface_texture, UINT32_MAX);

    bgfx_set_state(BGFX_STATE_DEFAULT_NO_CULL, 0);
    bgfx_submit(0, light_program.program, 0, BGFX_DISCARD_ALL);

    bgfx_frame(false);
  }

  destroy_shader_program(&shader_program);
  bgfx_destroy_vertex_buffer(vbh);
  bgfx_destroy_texture(container_texture);
  bgfx_destroy_texture(awesomeface_texture);
  bgfx_destroy_uniform(u_objectColor);
  bgfx_destroy_uniform(u_lightColor);
  bgfx_destroy_uniform(u_lightPos);
  bgfx_destroy_uniform(u_viewPos);
  bgfx_destroy_uniform(u_normalMatrix);
  bgfx_destroy_uniform(s_texture_1);
  bgfx_destroy_uniform(s_texture_2);

  bgfx_shutdown();

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
