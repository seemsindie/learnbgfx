$input v_normal, v_frag_position

#include "../../common/common.sh"

uniform vec4 u_objectColor;
uniform vec4 u_lightColor;
uniform vec4 u_lightPos;
uniform vec4 u_viewPos;

#define object_color u_objectColor.xyz
#define light_color u_lightColor.xyz
#define light_pos u_lightPos.xyz
#define view_pos u_viewPos.xyz

void main()
{
    // ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light_color;

    // diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light_pos - v_frag_position);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;

    // specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(view_pos - v_frag_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256); // chagne the specular power here it's 256 so we can see the effect better
    vec3 specular = specularStrength * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;
    gl_FragColor = vec4(result, 1.0);
}
