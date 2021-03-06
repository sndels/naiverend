#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    ~Camera() {;}

    void rotateTrackball(const glm::vec2& lasPos, const glm::vec2& curPos);
    void releaseTrackball();
    // Symmetric perspective projection, takes yFov as degrees
    void setProj(const float& xres, const float& yres, const float& yFov, const float& zN, const float& zF);
    // Eye is camera's world position, target the point in world to look at, up is global up
    void setView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f));
    void movePos(const glm::vec3& offset);
    void setPos(const glm::vec3& eye);
    glm::mat4 getViewMat() const;
    glm::mat4 getProjMat() const;

private:
    glm::vec2 viewDim2f_;
    glm::mat4 translationM4f_;
    glm::mat4 orientationM4f_;
    glm::mat4 projM4f_;

};

#endif
