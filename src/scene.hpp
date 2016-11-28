#ifndef SCENE_H
#define SCENE_H

#include "basic_program.hpp"
#include "camera.hpp"
#include "mesh.hpp"

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
    Mesh mesh_;

    glm::vec2 res2f_;
    glm::vec3 meshPos3f_;
    glm::vec3 meshScale3f_;

};

#endif
