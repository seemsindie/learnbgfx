#include <imgui/imgui_impl.h>

#define IMGUI_FLAGS_NONE UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

bgfx_view_id_t g_view_id = 255;
bgfx_texture_handle_t g_imgui_texture = BGFX_INVALID_HANDLE;
ImFont* regular_font = NULL;
ImFont* mono_font = NULL;
ImFont* kenney_font = NULL;
ImFont* awesome_font = NULL;

const ImWchar kenney_ranges[] = {ICON_MIN_KI, ICON_MAX_KI, 0};
const ImWchar font_awesome_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTime = 0;

int32_t m_lastScroll = 0;

int display_w = 0;
int display_h = 0;

void imgui_init(float scale) {
  ImGuiContext* ctx = igCreateContext(NULL);
  ImGuiIO* io = igGetIO();
  ImGuiStyle* style = igGetStyle();

  NOW = SDL_GetPerformanceCounter();

  style->WindowRounding = 0.0f;
  style->FrameRounding = 0.0f;

  io->IniFilename = NULL;
  io->DeltaTime = 1.0f / 60.0f;
  io->DisplaySize = (ImVec2){800, 600};
  io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

  // igStyleColorsLight(style);
  igStyleColorsDark(style);

  // load shaders
  s_imgui =
      load_shader_embedded(vs_ocornut_imgui_mtl, sizeof(vs_ocornut_imgui_mtl),
                           fs_ocornut_imgui_mtl, sizeof(fs_ocornut_imgui_mtl));
  s_imgui_image =
      load_shader_embedded(vs_imgui_image_mtl, sizeof(vs_imgui_image_mtl),
                           fs_imgui_image_mtl, sizeof(fs_imgui_image_mtl));

  // create vertex layout
  bgfx_vertex_layout_begin(&layout, bgfx_get_renderer_type());
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_POSITION, 2,
                         BGFX_ATTRIB_TYPE_FLOAT, false, false);
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_TEXCOORD0, 2,
                         BGFX_ATTRIB_TYPE_FLOAT, false, false);
  bgfx_vertex_layout_add(&layout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8,
                         true, false);
  bgfx_vertex_layout_end(&layout);

  // create uniform
  s_tex = bgfx_create_uniform("s_tex", BGFX_UNIFORM_TYPE_SAMPLER, 1);
  u_imageLodEnabled =
      bgfx_create_uniform("u_imageLodEnabled", BGFX_UNIFORM_TYPE_VEC4, 1);

  // fonts...
  ImFontConfig* config = ImFontConfig_ImFontConfig();
  // ImFontConfig config;
  config->FontDataOwnedByAtlas = false;
  config->MergeMode = false;
  const ImWchar* ranges = ImFontAtlas_GetGlyphRangesCyrillic(io->Fonts);

  regular_font = ImFontAtlas_AddFontFromMemoryTTF(
      io->Fonts, (void*)s_robotoRegularTtf, sizeof(s_robotoRegularTtf), 16.0f,
      config, ranges);
  mono_font = ImFontAtlas_AddFontFromMemoryTTF(
      io->Fonts, (void*)s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf),
      14.0f, config, ranges);

  config->MergeMode = true;

  // Load Kenney font
  config->DstFont = regular_font;

  // config->SizePixels = 16.0f * scale;
  // io->FontGlobalScale = 1.0f;
  // ctx->CurrentDpiScale = scale;
  // io->FontGlobalScale = 1.0f/scale;
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  display_w = dm.w;
  display_h = dm.h;

  kenney_font = ImFontAtlas_AddFontFromMemoryTTF(
      io->Fonts, (void*)s_iconsKenneyTtf, sizeof(s_iconsKenneyTtf), 16.0f,
      config, kenney_ranges);
  awesome_font = ImFontAtlas_AddFontFromMemoryTTF(
      io->Fonts, (void*)s_iconsFontAwesomeTtf, sizeof(s_iconsFontAwesomeTtf),
      16.0f, config, font_awesome_ranges);

  uint8_t* data;
  int32_t width;
  int32_t height;

  // void ImFontAtlas_GetTexDataAsRGBA32(ImFontAtlas *self, unsigned char
  // **out_pixels, int *out_width, int *out_height, int *out_bytes_per_pixel)
  ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &data, &width, &height, NULL);

  // printf("data: %p, width: %d, height: %d\n", data, width, height);

  g_imgui_texture =
      bgfx_create_texture_2d(width, height, false, 1, BGFX_TEXTURE_FORMAT_RGBA8,
                             0, bgfx_make_ref(data, width * height * 4));

  io->Fonts->TexID = (void*)(uintptr_t)g_imgui_texture.idx;
}

void imgui_shutdown() {
  igDestroyContext(NULL);
  destroy_shader_program(&s_imgui);
  destroy_shader_program(&s_imgui_image);
  bgfx_destroy_uniform(s_tex);
  bgfx_destroy_uniform(u_imageLodEnabled);
  bgfx_destroy_texture(g_imgui_texture);
}

void imgui_new_frame(int32_t _mx, int32_t _my, uint8_t _button, int32_t _scroll,
                     uint16_t _width, uint16_t _height, int _inputChar,
                     bgfx_view_id_t _viewId, float scale) {
  g_view_id = _viewId;

  LAST = NOW;
  NOW = SDL_GetPerformanceCounter();
  deltaTime =
      (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) *
      0.001;

  ImGuiIO* io = igGetIO();
  if (_inputChar >= 0) {
    ImGuiIO_AddInputCharacter(io, _inputChar);
  }

  // io->DisplaySize = (ImVec2){(float)display_w, (float)display_h};
  io->DisplaySize = (ImVec2){(float)_width, (float)_height};
  // io->DisplayFramebufferScale = (ImVec2){scale, scale};
  io->DisplayFramebufferScale = (ImVec2){1.0f, 1.0f};
  io->DeltaTime = deltaTime;

  ImGuiIO_AddMousePosEvent(io, _mx, _my);
  ImGuiIO_AddMouseButtonEvent(io, ImGuiMouseButton_Left,
                              0 != (_button & IMGUI_MBUT_LEFT));
  ImGuiIO_AddMouseButtonEvent(io, ImGuiMouseButton_Right,
                              0 != (_button & IMGUI_MBUT_RIGHT));
  ImGuiIO_AddMouseButtonEvent(io, ImGuiMouseButton_Middle,
                              0 != (_button & IMGUI_MBUT_MIDDLE));
  ImGuiIO_AddMouseWheelEvent(io, 0.0f, (float)(_scroll - m_lastScroll));
  m_lastScroll = _scroll;

  igNewFrame();
}

void imgui_end_frame(float scale) {
  igRender();

  ImDrawData* drawData = igGetDrawData();
  imgui_render(drawData, scale);
}

void imgui_render(ImDrawData* draw_data, float scale) {
  int fb_width =
      (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
  int fb_height =
      (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
  if (fb_width <= 0 || fb_height <= 0) return;
  
  // fb_width = (int)(fb_width * scale);
  // fb_height = (int)(fb_height * scale);

  const char* name = "ImGui";
  int32_t name_len = (int32_t)strlen(name);
  bgfx_set_view_name(g_view_id, name, name_len);
  bgfx_set_view_mode(g_view_id, BGFX_VIEW_MODE_SEQUENTIAL);

  mat4 ortho;
  vec4 bounds;
  float x = draw_data->DisplayPos.x;
  float y = draw_data->DisplayPos.y;
  float width = draw_data->DisplaySize.x;
  float height = draw_data->DisplaySize.y;

  // static inline void glm_ortho(float left, float right, float bottom, float
  // top, float nearZ, float farZ, vec4 *dest)
  glm_ortho(x, x + width, y + height, y, 0.0f, 1000.0f, ortho);

  bgfx_set_view_transform(g_view_id, NULL, ortho);
  bgfx_set_view_rect(g_view_id, 0, 0, (uint16_t)width, (uint16_t)height);

  const ImVec2 clipPos = draw_data->DisplayPos;
  const ImVec2 clipScale = draw_data->FramebufferScale;

  // Render command lists
  for (int32_t ii = 0, num = draw_data->CmdListsCount; ii < num; ++ii) {
    bgfx_transient_vertex_buffer_t tvb;
    bgfx_transient_index_buffer_t tib;

    const ImDrawList* cmd_list = draw_data->CmdLists.Data[ii];
    uint32_t vtx_size = (uint32_t)(cmd_list->VtxBuffer.Size);
    uint32_t idx_size = (uint32_t)(cmd_list->IdxBuffer.Size);

    if (!checkAvailTransientBuffers(vtx_size, &layout, idx_size)) {
      // not enough space in transient buffer, quit drawing the rest...
      break;
    }

    bgfx_alloc_transient_vertex_buffer(&tvb, vtx_size, &layout);
    bgfx_alloc_transient_index_buffer(&tib, idx_size, sizeof(ImDrawIdx) == 4);

    ImDrawVert* verts = (ImDrawVert*)tvb.data;
    memcpy(verts, cmd_list->VtxBuffer.Data, vtx_size * sizeof(ImDrawVert));

    ImDrawIdx* indices = (ImDrawIdx*)tib.data;
    memcpy(indices, cmd_list->IdxBuffer.Data, idx_size * sizeof(ImDrawIdx));

    bgfx_encoder_t* encoder = bgfx_encoder_begin(true);
    bgfx_encoder_touch(encoder, g_view_id);

    int cmd_count = cmd_list->CmdBuffer.Size;
    const ImDrawCmd* pcmd = cmd_list->CmdBuffer.Data;

    for (int32_t cmd_i = 0; cmd_i < cmd_count; ++cmd_i) {
      // printf("ElemCount: %d, TextureId: %p, VtxOffset: %d, IdxOffset: %d\n",
      //        pcmd->ElemCount, (void*)pcmd->TextureId, pcmd->VtxOffset,
      //        pcmd->IdxOffset);
      if (pcmd->UserCallback) {
        pcmd->UserCallback(cmd_list, pcmd);
      } else if (0 != pcmd->ElemCount) {
        uint64_t state =
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;
        uint32_t sampler_state = 0;

        bgfx_texture_handle_t th = g_imgui_texture;
        bgfx_program_handle_t program = s_imgui.program;
        int alphaBlend = 1;

        if (pcmd->TextureId != NULL) {
          uintptr_t textureInfo = (uintptr_t)pcmd->TextureId;
          if (textureInfo & BGFX_TEXTURE_FLAG_OPAQUE) {
            alphaBlend = 0;
          }
          if (textureInfo & BGFX_TEXTURE_FLAG_POINT_SAMPLER) {
            sampler_state = BGFX_SAMPLER_POINT;
          }
          textureInfo &= ~BGFX_TEXTURE_FLAG_ALL;
          th.idx = (uint16_t)textureInfo;
        }
        
        if (alphaBlend) {
          state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA,
                                         BGFX_STATE_BLEND_INV_SRC_ALPHA);
        }

        ImVec4 clipRect;
        clipRect.x = (pcmd->ClipRect.x - clipPos.x) * clipScale.x;
        clipRect.y = (pcmd->ClipRect.y - clipPos.y) * clipScale.y;
        clipRect.z = (pcmd->ClipRect.z - clipPos.x) * clipScale.x;
        clipRect.w = (pcmd->ClipRect.w - clipPos.y) * clipScale.y;

        // ImVec4 clipRect;
        // clipRect.x = (pcmd->ClipRect.x - clipPos.x) * clipScale.x * scale;
        // clipRect.y = (pcmd->ClipRect.y - clipPos.y) * clipScale.y * scale;
        // clipRect.z = (pcmd->ClipRect.z - clipPos.x) * clipScale.x * scale;
        // clipRect.w = (pcmd->ClipRect.w - clipPos.y) * clipScale.y * scale;

        if (clipRect.x < fb_width && clipRect.y < fb_height &&
            clipRect.z >= 0.0f && clipRect.w >= 0.0f) {
          // use float max from C not bx
          const uint16_t xx = (uint16_t)fmax(clipRect.x, 0.0f);
          const uint16_t yy = (uint16_t)fmax(clipRect.y, 0.0f);

          // printf("clipRect: %f %f %f %f\n", clipRect.x, clipRect.y,
          // clipRect.z, clipRect.w);

          // bgfx_set_scissor(xx, yy,
          //                  (uint16_t)fmin(clipRect.z, 65535.0f) - xx,
          //                  (uint16_t)fmin(clipRect.w, 65535.0f) - yy);
          bgfx_encoder_set_scissor(encoder, xx, yy,
                                   (uint16_t)fmin(clipRect.z, 65535.0f) - xx,
                                   (uint16_t)fmin(clipRect.w, 65535.0f) - yy);
          bgfx_encoder_set_state(encoder, state, 0);
          bgfx_encoder_set_texture(encoder, 0, s_tex, th, UINT32_MAX);
          bgfx_encoder_set_transient_vertex_buffer(encoder, 0, &tvb,
                                                   pcmd->VtxOffset, vtx_size);
          bgfx_encoder_set_transient_index_buffer(
              encoder, &tib, pcmd->IdxOffset, pcmd->ElemCount);
          bgfx_encoder_submit(encoder, g_view_id, program, 0, BGFX_DISCARD_ALL);
        }

        pcmd++;
      }

      bgfx_encoder_end(encoder);
    }
  }
}

bool checkAvailTransientBuffers(uint32_t _numVertices,
                                const bgfx_vertex_layout_t* _layout,
                                uint32_t _numIndices) {
  bool verticesAvailable =
      _numVertices ==
      bgfx_get_avail_transient_vertex_buffer(_numVertices, _layout);
  bool indicesAvailable = (_numIndices == 0) ||
                          (_numIndices == bgfx_get_avail_transient_index_buffer(
                                              _numIndices, false));

  return verticesAvailable && indicesAvailable;
}