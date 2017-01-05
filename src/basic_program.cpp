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
        glDeleteShader(vertexShader);
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

    modelToClipLoc_ = getUniformLocation("uModelToClip");
    if (modelToClipLoc_ == -1) return false;
    
    modelToCamLoc_ = getUniformLocation("uModelToCam");
    if (modelToCamLoc_ == -1) return false;

    normalToCamLoc_ = getUniformLocation("uNormalToCam");
    if (normalToCamLoc_ == -1) return false;
    
    diffuseColLoc_ = getUniformLocation("uDiffuseCol");
    if (diffuseColLoc_ == -1) return false;
    
    hasDiffuseTexLoc_ = getUniformLocation("uHasDiffuseTex");
    if (hasDiffuseTexLoc_ == -1) return false;
    
    toLightLoc_ = getUniformLocation("uToLight");
    if (toLightLoc_ == -1) return false;

    return true;
}

void BasicProgram::updateNormalToCam(const glm::mat3& ntc)
{
    glUniformMatrix3fv(normalToCamLoc_, 1, GL_FALSE, glm::value_ptr(ntc));
}

void BasicProgram::updateModelToCam(const glm::mat4& ptc)
{
    glUniformMatrix4fv(modelToCamLoc_, 1, GL_FALSE, glm::value_ptr(ptc));
}

void BasicProgram::updateModelToClip(const glm::mat4& ptc)
{
    glUniformMatrix4fv(modelToClipLoc_, 1, GL_FALSE, glm::value_ptr(ptc));
}

void BasicProgram::updateDiffuseCol(const glm::vec3& df)
{
    glUniform3fv(diffuseColLoc_, 1, glm::value_ptr(df));
}

void BasicProgram::updateHasDiffuseTex(const bool& b)
{
    glUniform1i(hasDiffuseTexLoc_, b);
}

void BasicProgram::updateToLight(const glm::vec3& l)
{
    glUniform3fv(toLightLoc_, 1, glm::value_ptr(l));
}

int32_t BasicProgram::getUniformLocation(const char* uniformName) const {
    int32_t uniformLocation = glGetUniformLocation(progID_, uniformName);
    if (uniformLocation == -1) {
        cerr << uniformName << " is not a valid shader variable" << endl;
    }
    return uniformLocation;
}
