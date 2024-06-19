#ifndef IMGUI_IMPL_H
#define IMGUI_IMPL_H
#include <bgfx/c99/bgfx.h>
#include <utils.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <cglm/cglm.h>
#include <stdbool.h>
#include <math.h>

// #include <cimgui/cimgui.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
// #include <imgui.h>

#include "droidsans.ttf.h"
#include "icons_font_awesome.ttf.h"
#include "robotomono_regular.ttf.h"
#include "roboto_regular.ttf.h"
#include "vs_imgui_image.bin.h"
#include "fs_imgui_image.bin.h"
#include "fs_ocornut_imgui.bin.h"
#include "vs_ocornut_imgui.bin.h"
#include "icons_kenney.h"
#include "icons_kenney.ttf.h"
#include "icons_font_awesome.h"

#define IMGUI_MBUT_LEFT   0x01
#define IMGUI_MBUT_RIGHT  0x02
#define IMGUI_MBUT_MIDDLE 0x04
#define BGFX_TEXTURE_FLAG_OPAQUE        (1u << 31)
#define BGFX_TEXTURE_FLAG_POINT_SAMPLER (1u << 30)
#define BGFX_TEXTURE_FLAG_ALL           (BGFX_TEXTURE_FLAG_OPAQUE | BGFX_TEXTURE_FLAG_POINT_SAMPLER)

// bgfx_view_id_t g_view_id = 255;
// extern bgfx_view_id_t g_view_id;

Shader s_imgui;
Shader s_imgui_image;
bgfx_vertex_layout_t layout;
bgfx_uniform_handle_t s_tex;
bgfx_uniform_handle_t u_imageLodEnabled;

void imgui_init(float);
void imgui_shutdown();
void imgui_new_frame(int32_t _mx, int32_t _my, uint8_t _button, int32_t _scroll, uint16_t _width, uint16_t _height, int _inputChar, bgfx_view_id_t _viewId, float scale);
void imgui_render(ImDrawData* draw_data, float);
void imgui_end_frame(float);
bool checkAvailTransientBuffers(uint32_t _numVertices, const bgfx_vertex_layout_t* _layout, uint32_t _numIndices);

#endif  // IMGUI_IMPL_H