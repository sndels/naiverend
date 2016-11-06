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
    void updateMVP(const glm::mat4& mvp);

private:
    int32_t getUniformLocation(const char* uniformName) const;

    int32_t mvpLoc_;
    int32_t nToEyeLoc_;

};

#endif
