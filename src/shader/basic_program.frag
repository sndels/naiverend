#version 410

// Transformation
uniform mat3 uNormalToCam;

// Material
uniform vec3 uDiffuseCol;
uniform vec3 uSpecularCol;
uniform float uSpecularExp;

// Textures
uniform bool uHasDiffuseTex;
uniform bool uHasNormalMap;
uniform sampler2D diffuseSampler;
uniform sampler2D normalSampler;

// Lights
uniform vec3 uToLight;

// Interpolated inputs
in vec3 posVar;
in vec3 normVar;
in vec2 texVar;

out vec4 fragColor;

const float lightIntensity = 0.15f;


void main()
{
    // Diffuse
    vec4 diffuseCol;
    if (uHasDiffuseTex) {
        diffuseCol = (texture(normalSampler, texVar));
    } else {
        diffuseCol = vec4(uDiffuseCol, 1);
    }
    
    // Lighting
    vec3 sumCol = vec3(0);

    vec3 N = normalize(uNormalToCam * normVar);
    vec3 V = -normalize(posVar);
    vec3 diffuse = diffuseCol.xyz * max(0, dot(N, uToLight));
    
    sumCol += diffuse;
    
    fragColor = vec4(sumCol, 1);
}
