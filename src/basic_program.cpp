#include "basic_program.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using std::cerr;
using std::endl;

bool BasicProgram::loadProgram() {
    progID_ = glCreateProgram();
    //Load and attacth shaders
    uint32_t vertexShader = loadShaderFromFile("shader/basic_program.vert",
                                             GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        glDeleteProgram(progID_);
        progID_ = 0;
        return false;
    }
    glAttachShader(progID_, vertexShader);
    uint32_t fragmentShader = loadShaderFromFile("shader/basic_program.frag",
                                               GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        glDeleteShader(progID_);
        glDeleteProgram(progID_);
        progID_ = 0;
        return false;
    }
    glAttachShader(progID_, fragmentShader);
    //Link program
    glLinkProgram(progID_);
    int32_t programSuccess = GL_FALSE;
    glGetProgramiv(progID_, GL_LINK_STATUS, &programSuccess);
    if (programSuccess == GL_FALSE) {
        cerr << "Error linking program " << progID_ << endl;
        cerr << "Error code: " << programSuccess;
        printProgramLog(progID_);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(progID_);
        progID_ = 0;
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    mvpLoc_ = getUniformLocation("uMVP");
    if (mvpLoc_ == -1) {
        cerr << "uMVP is not a valid uniform!" << endl;
        return false;
    }

    return true;
}

void BasicProgram::updateMVP(const glm::mat4& mvp)
{
    glUniformMatrix4fv(mvpLoc_, 1, GL_FALSE, glm::value_ptr(mvp));
}

int32_t BasicProgram::getUniformLocation(const char* uniformName) const {
    int32_t uniformLocation = glGetUniformLocation(progID_, uniformName);
    if (uniformLocation == -1) {
        cerr << uniformName << " is not a valid shader variable" << endl;
    }
    return uniformLocation;
}
