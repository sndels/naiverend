#version 410

layout(location = 0) in vec3 pos0;
layout(location = 1) in vec3 norm0;

out vec4 color0;

void main()
{
    gl_Position = vec4(pos0, 1.0);
    color0 = vec4(clamp(pos0, 0.0, 1.0), 1.0);
}
