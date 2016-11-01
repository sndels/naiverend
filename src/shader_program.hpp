#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "gl_core_4_1.h"
#include <string>

class ShaderProgram
{
public:
    ShaderProgram();
    virtual ~ShaderProgram();

    virtual bool loadProgram() = 0;
    virtual void freeProgram();
    uint32_t loadShaderFromFile(std::string path, GLenum shaderType);
    bool bind();
    uint32_t getProgramID();

protected:
    void printProgramLog(uint32_t program);
    void printShaderLog(uint32_t shader);

    uint32_t progID_;

};

#endif
