#include "scene.hpp"

#include "gl_core_4_1.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "mesh_parser.hpp"

using glm::vec3;
using std::cerr;
using std::cin;
using std::endl;

bool Scene::init()
{
    if (!pg_.loadProgram()) return false;

    parseOBJ("res/bunny.obj", mesh_);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        cerr << "Error initializing scene!" << endl;
        cerr << "Error code: " << error << endl;
        cin.get();
        return false;
    }

    meshPos3f_ = vec3(0.125f, -0.5f, 0.f);
    meshScale3f_ = vec3(0.5f, 0.5f, 0.5f);
    return true;
}
void Scene::update()
{
    ;
}

void Scene::render()
{
    pg_.bind();
    glm::mat4 model(meshScale3f_.x,            0.f,            0.f, 0.f,
                               0.f, meshScale3f_.y,            0.f, 0.f,
                               0.f,            0.f, meshScale3f_.z, 0.f,
                      meshPos3f_.x,   meshPos3f_.y,   meshPos3f_.z, 1.f);
    pg_.updateMVP(model);
    mesh_.render();
}
