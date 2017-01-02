#version 410
uniform mat3 uNormalToCam;

in vec3 position0;
in vec3 normal0;

out vec4 fragColor;

const float cShadingIntensity = 1.0;
const vec3 cToLight = normalize(vec3(5.0, 7.0, 5.0));
const vec4 cModelColor = vec4(0.8, 0.8, 0.8, 1.0);

void main()
{
    vec3 mappedNormal = normalize(uNormalToCam * normal0);
    vec3 eye = -normalize(position0);
    vec3 light = normalize(uNormalToCam * cToLight);
    float clampedCosine = clamp(dot(mappedNormal, light), 0.0, 1.0);
    vec4 litColor = vec4(vec3(clampedCosine), 1.0);
    fragColor = cModelColor * litColor;
}
