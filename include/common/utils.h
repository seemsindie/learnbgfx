#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL_image.h>
#include <bgfx/c99/bgfx.h>
#include <setup_metal_layer.h>

typedef struct Shader {
    bgfx_shader_handle_t vertex_shader;
    bgfx_shader_handle_t fragment_shader;
    bgfx_program_handle_t program;
} Shader;

SDL_Window* init_sdl_bgfx(uint32_t width, uint32_t height, bgfx_init_t* init, uint32_t debug, uint32_t reset);
void process_input(SDL_Event* event, bool* running, uint32_t* width, uint32_t* height,
                         uint32_t reset, bgfx_init_t init);
bgfx_shader_handle_t load_shader(const char* file_path);
Shader load_shaders_bin(const char* vertex_shader_path, const char* fragment_shader_path);
Shader load_shader_embedded(const uint8_t* vertex_shader, const uint32_t vs_size, const uint8_t* fragment_shader, const uint32_t fs_size);
void destroy_shader_program(Shader* shader);
SDL_Surface* load_image(const char* filename);
bgfx_texture_handle_t create_texture_from_surface(SDL_Surface* surface);

#endif  // UTILS_H
