$input v_color0, v_texcoord0

#include "../../common/common.sh"

SAMPLER2D(s_texture_1, 0);
SAMPLER2D(s_texture_2, 1);

void main()
{
    gl_FragColor = v_color0 * mix(texture2D(s_texture_1, v_texcoord0), texture2D(s_texture_2, v_texcoord0), 0.2);
    //gl_FragColor = v_color0;
}
