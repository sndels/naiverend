#include "scene.hpp"

#include "gl_core_4_1.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "input_handler.hpp"
#include "model_parser.hpp"

using glm::vec3;
using glm::mat3;
using glm::mat4;
using std::cerr;
using std::cin;
using std::endl;

Scene::Scene(const float& xres, const float& yres) :
    res2f_(xres, yres),
    modelPos3f_(0.f, 0.f, 0.f),
    modelScale3f_(10.f)
{
    ;
}

bool Scene::init()
{
    if (!pg_.loadProgram()) return false;

    parseOBJ("res/bunny.obj", model_);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        cerr << "Error initializing scene!" << endl;
        cerr << "Error code: " << error << endl;
        cin.get();
        return false;
    }

    cam_.setProj(res2f_.x, res2f_.y, 90.f, 0.1f, 10.f);
    cam_.setView(vec3(0.f, 0.f, -4.f), vec3(0.f, 0.f, 0.f));

    modelPos3f_ = vec3(0.f, 0.f, 0.f);
    modelScale3f_ = vec3(1.f, 1.f, 1.f);
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
    vec3 modelOffset(0.f, 0.f, 0.f);
    if (ks.shift) {
        if (ks.up) modelOffset.z += 1.f;
        if (ks.down) modelOffset.z -= 1.f;
    } else {
        if (ks.up) modelOffset.y += 1.f;
        if (ks.down) modelOffset.y -= 1.f;
    }
    if (ks.left) modelOffset.x -= 1.f;
    if (ks.right) modelOffset.x += 1.f;

    if (length(modelOffset) > 0.f) modelPos3f_ += 0.05f * normalize(modelOffset);

    ih.reset();
}

void Scene::render()
{
    pg_.bind();
    mat4 translate(          1.f,           0.f,           0.f, 0.f,
                             0.f,           1.f,           0.f, 0.f,
                             0.f,           0.f,           1.f, 0.f,
                   modelPos3f_.x, modelPos3f_.y, modelPos3f_.z, 1.f );
    mat4 scale(modelScale3f_.x,            0.f,              0.f, 0.f,
                           0.f, modelScale3f_.y,             0.f, 0.f,
                           0.f,             0.f, modelScale3f_.z, 0.f,
                           0.f,             0.f,             0.f, 1.f );

    mat4 viewMat = cam_.getViewMat();
    mat4 projMat = cam_.getProjMat();
    
    mat4 posToCam = viewMat * translate * scale;
    pg_.updatePosToCam(posToCam);
    mat3 posToCam3x3(posToCam[0][0], posToCam[0][1], posToCam[0][2],
                     posToCam[1][0], posToCam[1][1], posToCam[1][2],
                     posToCam[2][0], posToCam[2][1], posToCam[2][2] );
    pg_.updateNormalToCam(transpose(inverse(posToCam3x3)));
    mat4 posToClip = projMat * posToCam;
    pg_.updatePosToClip(posToClip);
    model_.render();
}
