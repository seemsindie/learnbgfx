$input a_position, a_normal
$output v_normal, v_frag_position

uniform vec4 u_color;

#include "../../common/common.sh"

void main()
{
    // gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_frag_position = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    // v_normal = a_normal;
    v_normal = normalize(mul(u_model[0], vec4(a_normal.xyz, 0.0) ).xyz);
    // v_normal = mul(mat3(transpose(inverse(u_modelView))), a_normal);
    // v_normal = mul(u_model[0], vec4(a_normal, 0.0)).xyz;
    // v_normal = mul(mat3(transpose(inverse(u_model[0]))), a_normal);
}
