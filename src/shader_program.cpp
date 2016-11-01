#include "shader_program.hpp"

#include <iostream>
#include <fstream>
#include <exception>

using std::cerr;
using std::endl;

ShaderProgram::ShaderProgram() :
    progID_(0)
{;}

ShaderProgram::~ShaderProgram()
{
    freeProgram();
}

void ShaderProgram::freeProgram()
{
    glDeleteProgram(progID_);
}

uint32_t ShaderProgram::loadShaderFromFile(std::string path, GLenum shaderType)
{
    uint32_t shaderID = 0;
    std::ifstream sourceFile(path.c_str());
    if (sourceFile) {
        std::string shaderString((std::istreambuf_iterator<char>(sourceFile)),
                                  std::istreambuf_iterator<char>());
        shaderID = glCreateShader(shaderType);
        const GLchar* shaderSource = shaderString.c_str();
        glShaderSource(shaderID, 1, &shaderSource, NULL);
        glCompileShader(shaderID);
        int32_t shaderCompiled = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled == GL_FALSE) {
            cerr << "Unable to compile shader " << shaderID << '!' << endl;
            printShaderLog(shaderID);
            shaderID = 0;
        }
    } else {
        cerr << "unable to open file " << path << endl;
    }
    return shaderID;
}

bool ShaderProgram::bind()
{
    glUseProgram(progID_);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        cerr << "Error binding shader!" << endl;
        cerr << "Error code: " << error << endl;
        printProgramLog(progID_);
        return false;
    }
    return true;
}

void ShaderProgram::printProgramLog(uint32_t program)
{
    if (glIsProgram(program) == GL_TRUE) {
        int32_t maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        char* errorLog = new char[maxLength];
        glGetProgramInfoLog(program, maxLength, &maxLength, errorLog);
        for (auto i = 0; i < maxLength; ++i)
            cerr << errorLog[i];
        cerr << endl;
        delete[] errorLog;
    } else {
        cerr << "Name " << program << " is not a program!" << endl;
    }
}

void ShaderProgram::printShaderLog(uint32_t shader)
{
    if (glIsShader(shader) == GL_TRUE) {
        int32_t maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        char* errorLog = new char[maxLength];
        glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
        for (auto i = 0; i < maxLength; ++i)
            cerr << errorLog[i];
        cerr << endl;
        delete[] errorLog;
    } else {
        cerr << "Name " << shader << " is not a shader!" << endl;
    }
}
