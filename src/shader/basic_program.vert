#version 410

layout(location = 0) in vec3 pos0;
layout(location = 1) in vec3 norm0;

uniform mat4 uPosToClip;
uniform mat4 uPosToCam;

out vec3 position0;
out vec3 normal0;

void main()
{
    position0 = (uPosToCam * vec4(pos0, 1.0)).xyz;
    normal0 = norm0;
    gl_Position  = uPosToClip * vec4(pos0, 1.0);
}
