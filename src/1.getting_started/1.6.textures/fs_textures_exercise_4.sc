$input v_color0, v_texcoord0

#include "../../common/common.sh"

SAMPLER2D(s_texture_1, 0);
SAMPLER2D(s_texture_2, 1);

uniform vec4 u_mix_amount;

void main()
{
    vec2 flipped_texcoord = vec2(1.0 - v_texcoord0.x, v_texcoord0.y);
    float mix_value = u_mix_amount.x;
    gl_FragColor = mix(texture2D(s_texture_1, v_texcoord0), texture2D(s_texture_2, flipped_texcoord), mix_value) * v_color0;
}
