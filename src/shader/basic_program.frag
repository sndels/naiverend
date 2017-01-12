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

const float lightIntensity = 0.8f;


void main()
{
    // Diffuse color
    vec3 diffuseCol;
    if (uHasDiffuseTex) {
        diffuseCol = texture(diffuseSampler, texVar).xyz;
    } else {
        diffuseCol = uDiffuseCol;
    }
    
    // Normals
    vec3 mappedNormal;
    if (uHasNormalMap) {
        mappedNormal = (texture(normalSampler, texVar) * 2.f - 1.f).xyz;
    } else {
        mappedNormal = normVar;
    }
    
    // Lighting
    vec3 sumCol = vec3(0);

    vec3 N = normalize(uNormalToCam * mappedNormal);
    vec3 diffuse = diffuseCol.xyz * max(0, dot(N, uToLight));
   
    vec3 V = -normalize(posVar);
    vec3 h = normalize(V + uToLight);
    vec3 specular = uSpecularCol * pow(max(0, dot(N, h)), uSpecularExp);
    
    sumCol += lightIntensity * (diffuse + specular);
    
    fragColor = vec4(sumCol, 1);
}
