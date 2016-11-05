#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    ~Camera() {;}

    void rotateTrackball(const glm::vec2& startPos, const glm::vec2& lastPos);
    // Symmetric perspective projection, takes yFov as degrees
    void setProj(const float& xres, const float& yres, const float& yFov, const float& zN, const float& zF);
    // Eye is camera's world position, target the point in world to look at, up is global up
    void setView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 getVP() const;

private:
    glm::vec3 worldPos3f;
    glm::mat3 orientationM3f_;
    glm::mat4 projM4f_;
    glm::mat4 viewM4f_;

};

#endif