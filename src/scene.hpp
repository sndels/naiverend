#ifndef SCENE_H
#define SCENE_H

#include "basic_program.hpp"
#include "mesh.hpp"

class Scene
{
public:
    Scene() {;}
    ~Scene() {;}

    bool init();
    void update();
    void render();

private:
    BasicProgram pg_;
    Mesh mesh_;

    glm::vec3 meshPos3f_;
    glm::vec3 meshScale3f_;

};

#endif
