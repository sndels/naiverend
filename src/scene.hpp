#ifndef SCENE_H
#define SCENE_H

#include "basic_program.hpp"

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
    uint32_t vao_;
    uint32_t vbo_;

};

#endif
