$input v_normal, v_frag_position

#include "../../common/common.sh"

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * u_lightColor;

    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_lightPos - v_frag_position);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse) * u_objectColor;
    gl_FragColor = vec4(result, 1.0);
}
