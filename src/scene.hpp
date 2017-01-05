#ifndef SCENE_H
#define SCENE_H

#include "basic_program.hpp"
#include "camera.hpp"
#include "model.hpp"

class Scene
{
public:
    Scene(const float& xres, const float& yres);
    ~Scene() {;}

    bool init();
    void update();
    void render();

private:
    BasicProgram pg_;
    Camera cam_;
    Model model_;

    glm::vec2 res2f_;
    glm::vec3 modelPos3f_;
    glm::vec3 modelScale3f_;
    float modelRotY1f_;
    glm::vec3 lightPos3f_;

};

#endif
