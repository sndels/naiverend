#include "camera.hpp"

#include "math_types.hpp"

using glm::mat3;
using glm::mat4;
using glm::vec3;

Camera::Camera() :
    worldPos3f(0.f, 0.f, -3.f),
    orientationM3f_(1.f),
    projM4f_(1.f),
    viewM4f_(1.f)
{
    ;
}

void Camera::rotateTrackball(const glm::vec2& startPos, const glm::vec2& lastPos)
{
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
    vec3 f = normalize(target - eye);
    vec3 r = normalize(cross(up, f));
    vec3 u = normalize(cross(f, r));
    orientationM3f_ = mat3(r.x, u.x, f.x,
                           r.y, u.y, f.y,
                           r.z, u.z, f.z );
    worldPos3f = eye;
    viewM4f_ = mat4(         r.x,          u.x,          f.x, 0.f,
                             r.y,          u.y,          f.y, 0.f,
                             r.z,          u.z,          f.z, 0.f,
                    -dot(r, eye), -dot(u, eye), -dot(f, eye), 1.f );
}

glm::mat4 Camera::getVP() const
{
    return projM4f_ * viewM4f_;
}

