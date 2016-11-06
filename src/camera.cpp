#include "camera.hpp"

#include <glm/gtx/transform.hpp>
#include "math_types.hpp"

using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;

namespace {

    mat4 startOrientation = mat4(1.f);

    vec3 screenToSphere(const vec2& p, const vec2& dim)
    {
        float r = 0.f;
        float xo = 0.f;
        float yo = 0.f;
        if (dim.y < dim.x) {
            r = dim.y;
            xo = dim.x - dim.y;
        } else {
            r = dim.x;
            yo = dim.y - dim.x;
        }
        vec3 v(0.f, 0.f, 0.f);
        v.x = (2.f*p.x - xo - r) / r;
        v.y = (yo + r - 2.f*p.y) / r;
        float d = length(v);
        v.z = cos((PI_F/2.f) * ((d < 1.f) ? d : 1.f));
        return normalize(v);
    }

}

Camera::Camera() :
    viewDim2f_(0.f, 0.f),
    translationM4f_(1.f),
    orientationM4f_(1.f),
    projM4f_(1.f)
{
    ;
}

void Camera::rotateTrackball(const glm::vec2& startPos, const glm::vec2& curPos)
{
    vec3 cv = screenToSphere(curPos, viewDim2f_);
    vec3 sv = screenToSphere(startPos, viewDim2f_);
    vec3 dv = cv - sv;
    if (dv.x || dv.y || dv.z) {
        vec3 axis = normalize(cross(cv, sv));
        float angle = acos(dot(cv, sv));
        orientationM4f_ = glm::rotate(angle, axis) * startOrientation;
    }
}

void Camera::releaseTrackball()
{
    startOrientation = orientationM4f_;
}

void Camera::setProj(const float& xres, const float& yres, const float& yFov, const float& zN, const float& zF)
{
    float ar = xres / yres;
    float tFov = (0.5f * yFov * PI_F / 180.f);
    projM4f_ = mat4(1.f / ar * tFov,        0.f,               0.f, 0.f,
                                0.f, 1.f / tFov,               0.f, 0.f,
                                0.f,        0.f,  (-zN-zF)/(zN-zF), 1.f,
                                0.f,        0.f, 2.f*zF*zN/(zN-zF), 0.f );
    viewDim2f_ = vec2(xres, yres);
}

void Camera::setView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
    vec3 f = normalize(target - eye);
    vec3 r = normalize(cross(up, f));
    vec3 u = normalize(cross(f, r));
    translationM4f_ = mat4(    1.f,    0.f,    0.f, 0.f,
                               0.f,    1.f,    0.f, 0.f,
                               0.f,    0.f,    1.f, 0.f,
                            -eye.x, -eye.y, -eye.z, 1.f );
    orientationM4f_ = mat4(r.x, u.x, f.x, 0.f,
                           r.y, u.y, f.y, 0.f,
                           r.z, u.z, f.z, 0.f,
                           0.f, 0.f, 0.f, 1.f );
    startOrientation = orientationM4f_;
}

glm::mat4 Camera::getVP() const
{
    return projM4f_ * translationM4f_ * orientationM4f_;
}
