$input v_normal, v_frag_position

#include "../../common/common.sh"

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform Material u_material;

void main()
{
    // ambient
    vec3 ambient = u_lightColor * u_material.ambient;

    // diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_lightPos - v_frag_position);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_lightColor * (diff * u_material.diffuse);

    // specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(u_viewPos - v_frag_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess); // chagne the specular power here it's 256 so we can see the effect better
    vec3 specular = u_lightColor * (spec * u_material.specular);

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    gl_FragColor = vec4(result, 1.0);
}
