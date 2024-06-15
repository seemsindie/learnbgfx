$input v_color0, v_texcoord0

#include "../../common/common.sh"

uniform vec4 u_objectColor;
uniform vec4 u_lightColor;

void main()
{
    gl_FragColor = vec4(u_objectColor * u_lightColor);
}
