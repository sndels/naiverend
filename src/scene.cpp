#include "scene.hpp"

#include "gl_core_4_1.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "input_handler.hpp"
#include "mesh_parser.hpp"

using glm::vec3;
using glm::mat4;
using std::cerr;
using std::cin;
using std::endl;

Scene::Scene(const float& xres, const float& yres) :
    res2f_(xres, yres),
    meshPos3f_(0.f, 0.f, 0.f),
    meshScale3f_(2.f, 2.f, 2.f)
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

    cam_.setProj(res2f_.x, res2f_.y, 90.f, 1.f, 10.f);
    cam_.setView(vec3(0.f, 0.f, -2.f), vec3(0.f, 0.f, 0.f));

    meshPos3f_ = vec3(0.f, 0.f, 0.f);
    meshScale3f_ = vec3(1.f, 1.f, 1.f);
    return true;
}

void Scene::update()
{
    InputHandler& ih = InputHandler::getIH();

    // Cam control
    const MouseState ms = ih.getMouseState();
    if (ms.state == LEFT_DOWN) cam_.rotateTrackball(ms.lastPos2f, ms.curPos2f);
    cam_.movePos(vec3(0.f, 0.f, ms.scrollY / 4.f));

    // Model control
    const KeyboardState ks = ih.getKeyboardState();
    vec3 meshOffset(0.f, 0.f, 0.f);
    if (ks.shift) {
        if (ks.up) meshOffset.z += 1.f;
        if (ks.down) meshOffset.z -= 1.f;
    } else {
        if (ks.up) meshOffset.y += 1.f;
        if (ks.down) meshOffset.y -= 1.f;
    }
    if (ks.left) meshOffset.x -= 1.f;
    if (ks.right) meshOffset.x += 1.f;

    if (length(meshOffset) > 0.f) meshPos3f_ += 0.05f * normalize(meshOffset);

    ih.reset();
}

void Scene::render()
{
    pg_.bind();
    mat4 translate(         1.f,          0.f,          0.f, 0.f,
                            0.f,          1.f,          0.f, 0.f,
                            0.f,          0.f,          1.f, 0.f,
                   meshPos3f_.x, meshPos3f_.y, meshPos3f_.z, 1.f );
    mat4 scale(meshScale3f_.x,            0.f,            0.f, 0.f,
                          0.f, meshScale3f_.y,            0.f, 0.f,
                          0.f,            0.f, meshScale3f_.z, 0.f,
                          0.f,            0.f,            0.f, 1.f );
    pg_.updateMVP(cam_.getVP() * translate * scale);
    mesh_.render();
}
