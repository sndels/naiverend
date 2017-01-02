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
    void updatePosToCam(const glm::mat4& ptc);
    void updatePosToClip(const glm::mat4& ptc);

private:
    int32_t getUniformLocation(const char* uniformName) const;

    int32_t posToClipLoc_;
    int32_t posToCamLoc_;
    int32_t normalToCamLoc_;

};

#endif
