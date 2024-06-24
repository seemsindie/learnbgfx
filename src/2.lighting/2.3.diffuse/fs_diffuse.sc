$input v_normal, v_frag_position

#include "../../common/common.sh"

uniform vec4 u_objectColor;
uniform vec4 u_lightColor;
uniform vec4 u_lightPos;

#define object_color u_objectColor.xyz
#define light_color u_lightColor.xyz
#define light_pos u_lightPos.xyz

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light_color;

    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light_pos - v_frag_position);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 result = (ambient + diffuse) * object_color;
    gl_FragColor = vec4(result, 1.0);
}
