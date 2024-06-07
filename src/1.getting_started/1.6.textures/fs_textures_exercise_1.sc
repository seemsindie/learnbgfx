$input v_color0, v_texcoord0

#include "../../common/common.sh"

SAMPLER2D(s_texture_1, 0);
SAMPLER2D(s_texture_2, 1);

void main()
{
    vec2 flipped_texcoord = vec2(1.0 - v_texcoord0.x, v_texcoord0.y);
    gl_FragColor = mix(texture2D(s_texture_1, v_texcoord0), texture2D(s_texture_2, flipped_texcoord), 0.2) * v_color0;
}
