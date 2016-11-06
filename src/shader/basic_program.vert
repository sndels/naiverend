#version 410

layout(location = 0) in vec3 pos0;
layout(location = 1) in vec3 norm0;

uniform mat4 uMVP;
uniform mat4 uNormalToEye;

out vec4 color0;

const float cShadingIntensity = 0.9;
const vec3 cToLight = normalize(vec3(0.5, 0.7, -0.5));
const vec3 cMeshColor = vec3(0.8, 0.8, 0.8);

void main()
{
    vec3 eyeNormal = normalize(vec3(uNormalToEye * vec4(norm0, 1.0)));
    float clampedCosine = clamp(dot(eyeNormal, cToLight), 0.0, 1.0);
    vec3 litColor = vec3(clampedCosine);
    gl_Position  = uMVP * vec4(pos0, 1.0);
    color0 = vec4(mix(cMeshColor, litColor, cShadingIntensity), 1.0);
}
