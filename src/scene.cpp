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

    std::vector<Vertex> verts = { {vec3( 0.f, -0.866f, -0.866f), vec3(0.f, 0.f, 0.f)},
                                  {vec3( 1.f, -0.866f,  0.866f), vec3(0.f, 0.f, 0.f)},
                                  {vec3(-1.f, -0.866f,  0.866f), vec3(0.f, 0.f, 0.f)},
                                  {vec3( 0.f,  0.866f,     0.f), vec3(0.f, 0.f, 0.f)} };
    std::vector<glm::u32vec3> faces = { glm::u32vec3(0, 1, 2),
                                        glm::u32vec3(0, 1, 3),
                                        glm::u32vec3(1, 2, 3),
                                        glm::u32vec3(2, 0, 3) };
    mesh_.update(verts, faces);

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
