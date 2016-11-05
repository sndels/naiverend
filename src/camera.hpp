#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    ~Camera() {;}

    // Symmetric perspective projection, takes yFov as degrees
    void setProj(const float& xres, const float& yres, const float& yFov, const float& zN, const float& zF);
    void setView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
    glm::mat4 getVP() const;

private:
    glm::mat4 projM4f_;
    glm::mat4 viewM4f_;

};

#endif