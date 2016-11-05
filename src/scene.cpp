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

Scene::Scene(const float& xres, const float& yres) :
    xres1f_(xres),
    yres1f_(yres),
    meshPos3f_(0.f, 0.f, 0.f),
    meshScale3f_(1.f, 1.f, 1.f)
{
    ;
}

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

    cam_.setProj(xres1f_, yres1f_, 90.f, 1.f, 10.f);

    meshPos3f_ = vec3(0.125f, -1.f, 2.f);
    meshScale3f_ = vec3(1.f, 1.f, 1.f);
    return true;
}
void Scene::update()
{
    ;
}

void Scene::render()
{
    pg_.bind();
    glm::mat4 translate(         1.f,          0.f,          0.f, 0.f,
                                 0.f,          1.f,          0.f, 0.f,
                                 0.f,          0.f,          1.f, 0.f,
                        meshPos3f_.x, meshPos3f_.y, meshPos3f_.z, 1.f );
    glm::mat4 scale(meshScale3f_.x,            0.f,            0.f, 0.f,
                               0.f, meshScale3f_.y,            0.f, 0.f,
                               0.f,            0.f, meshScale3f_.z, 0.f,
                               0.f,            0.f,            0.f, 1.f );
    pg_.updateMVP(cam_.getVP() * translate * scale);
    mesh_.render();
}
