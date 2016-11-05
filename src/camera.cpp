#include "camera.hpp"

#include "math_types.hpp"

using glm::mat4;

Camera::Camera() :
    projM4f_(1.f),
    viewM4f_(1.f)
{
    ;
}

void Camera::setProj(const float& xres, const float& yres, const float& yFov, const float& zN, const float& zF)
{
    float ar = xres / yres;
    float tFov = (0.5f * yFov * PI_F / 180.f);
    projM4f_ = mat4(1.f / ar * tFov,        0.f,               0.f, 0.f,
                                0.f, 1.f / tFov,               0.f, 0.f,
                                0.f,        0.f,  (-zN-zF)/(zN-zF), 1.f,
                                0.f,        0.f, 2.f*zF*zN/(zN-zF), 0.f );
}

void Camera::setView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
    ;
}

glm::mat4 Camera::getVP() const
{
    return projM4f_ * viewM4f_;
}

