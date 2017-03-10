#include "scene.hpp"

#include "gl_core_4_1.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "input_handler.hpp"
#include "math_types.hpp"
#include "model_parser.hpp"

#include "imgui/imgui.h"
#include "imgui/examples/opengl3_example/imgui_impl_glfw_gl3.h"

using glm::vec3;
using glm::mat3;
using glm::mat4;
using std::cerr;
using std::cin;
using std::endl;

Scene::Scene(const float& xres, const float& yres) :
    res2f_(xres, yres),
    modelPos3f_(0.f, 0.f, 0.f),
    modelScale3f_(1.f),
    modelRotY1f_(PI_F),
    lightPos3f_(5.f, 7.f, -5.f)
{
    ;
}

bool Scene::init()
{
    if (!pg_.loadProgram()) return false;

    parseOBJ("res/head/head.obj", model_);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        cerr << "Error initializing scene!" << endl;
        cerr << "Error code: " << error << endl;
        cin.get();
        return false;
    }

    cam_.setProj(res2f_.x, res2f_.y, 90.f, 0.1f, 10.f);
    cam_.setView(vec3(0.f, 0.f, -4.f), vec3(0.f, 0.f, 0.f));

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
    // Simple imgui-window
    // mousepos could be passed to imgui before event-handling and check this
    // to not pass mouse-events to program if hovering
    ImGui::GetIO().MouseDrawCursor = ImGui::IsMouseHoveringAnyWindow();
    {
        ImGui::SliderFloat("x", &modelPos3f_.x, -2.f, 2.f);
        ImGui::SliderFloat("y", &modelPos3f_.y, -2.f, 2.f);
        ImGui::SliderFloat("z", &modelPos3f_.z, -2.f, 2.f);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    pg_.bind();
    mat4 translate(          1.f,           0.f,           0.f, 0.f,
                             0.f,           1.f,           0.f, 0.f,
                             0.f,           0.f,           1.f, 0.f,
                   modelPos3f_.x, modelPos3f_.y, modelPos3f_.z, 1.f );
    mat4 scale(modelScale3f_.x,            0.f,              0.f, 0.f,
                           0.f, modelScale3f_.y,             0.f, 0.f,
                           0.f,             0.f, modelScale3f_.z, 0.f,
                           0.f,             0.f,             0.f, 1.f );
    
    mat4 rotY(cos(modelRotY1f_), 0.f, -sin(modelRotY1f_), 0.f,
                            0.f, 1.f,                0.f, 0.f,
              sin(modelRotY1f_), 0.f,  cos(modelRotY1f_), 0.f,
                            0.f, 0.f,                0.f, 1.f );

    mat4 modelToWorld = translate * rotY * scale;
    mat4 viewMat = cam_.getViewMat();
    mat4 projMat = cam_.getProjMat();
    
    mat4 modelToCam = viewMat * modelToWorld;
    mat4 modelToClip = projMat * modelToCam;
    mat3 normalToCam = transpose(inverse(mat3(modelToCam[0][0], modelToCam[0][1], modelToCam[0][2],
                                              modelToCam[1][0], modelToCam[1][1], modelToCam[1][2],
                                              modelToCam[2][0], modelToCam[2][1], modelToCam[2][2] )));
    
    mat3 worldToCam = transpose(inverse(mat3(viewMat[0][0], viewMat[0][1], viewMat[0][2],
                           viewMat[1][0], viewMat[1][1], viewMat[1][2],
                           viewMat[2][0], viewMat[2][1], viewMat[2][2] )));
    
    pg_.updateModelToCam(modelToCam);
    pg_.updateModelToClip(modelToClip);
    pg_.updateNormalToCam(normalToCam);
    pg_.updateToLight(normalize(worldToCam * lightPos3f_));
    model_.render(pg_);
}
