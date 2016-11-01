#include "scene.hpp"

#include "gl_core_4_1.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

using glm::vec3;
using std::cerr;
using std::cin;
using std::endl;

bool Scene::init()
{
    if (!pg_.loadProgram()) return false;

    std::vector<vec3> tri = { vec3(-1.f, -1.f, 0.f),
                              vec3(-1.f,  1.f, 0.f),
                              vec3( 1.f,  1.f, 0.f) };
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*3, tri.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        cerr << "Error initializing scene!" << endl;
        cerr << "Error code: " << error << endl;
        cin.get();
        return false;
    }
    return true;
}
void Scene::update()
{
    ;
}

void Scene::render()
{
    pg_.bind();
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}
