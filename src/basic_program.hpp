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
    int32_t getUniformLocation(const char* uniformName) const;

private:

};

#endif
