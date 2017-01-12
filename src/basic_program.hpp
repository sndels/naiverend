#ifndef BASICPROGRAM_H
#define BASICPROGRAM_H

#include "gl_core_4_1.h"
#include <glm/glm.hpp>
#include "shader_program.hpp"

class BasicProgram : public ShaderProgram
{
public:
    BasicProgram() {;}
    ~BasicProgram() {;}

    bool loadProgram();
    void updateNormalToCam(const glm::mat3& ntc);
    void updateModelToCam(const glm::mat4& ptc);
    void updateModelToClip(const glm::mat4& ptc);
    void updateDiffuseCol(const glm::vec3& df);
    void updateSpecularCol(const glm::vec3& s);
    void updateSpecularExp(const float& s);
    void updateHasDiffuseTex(const bool& b);
    void updateHasNormalMap(const bool& b);
    void updateToLight(const glm::vec3& l);

private:
    int32_t getUniformLocation(const char* uniformName) const;

    // Vert uniforms
    int32_t modelToClipLoc_;
    int32_t modelToCamLoc_;

    // Frag uniforms
    int32_t normalToCamLoc_;

    int32_t hasDiffuseTexLoc_;
    int32_t hasNormalMapLoc_;
    int32_t diffuseColLoc_;
    int32_t specularColLoc_;
    int32_t specularExpLoc_;
    
    int32_t toLightLoc_;

};

#endif
