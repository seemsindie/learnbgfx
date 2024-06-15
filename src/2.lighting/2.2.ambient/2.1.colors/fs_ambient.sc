$input v_color0, v_texcoord0

#include "../../common/common.sh"

uniform vec4 u_objectColor;
uniform vec4 u_lightColor;

void main()
{
    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * u_lightColor;

    vec4 result = ambient * u_objectColor;
    gl_FragColor = result;
}
