#version 410
layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout  (location = 1) in float mass;
layout (location = 2) in float charge;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out float v_charge;
out float v_mass;
flat out vec4 center;
void main()
{
    gl_PointSize=mass;
    v_mass = mass;
    center = projection*model*view*vec4(position, 1.0); // See how we directly give a vec3 to vec4's constructor
    v_charge = charge;
    gl_Position=center;
}
