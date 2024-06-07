$input a_position, a_color0
$output v_color0

uniform vec4 u_color;

#include "../../common/common.sh"

void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
    v_color0 = u_color;
}
