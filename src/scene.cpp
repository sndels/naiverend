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
    return true;
}
void Scene::update()
{
    ;
}

void Scene::render()
{
    pg_.bind();
    mesh_.render();
}
