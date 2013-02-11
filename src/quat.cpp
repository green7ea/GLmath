#include "quat.h"

#include <cmath>

#include "mat4f.h"

Quat::Quat(float w, float x, float y, float z)
    : w(w),
      x(x),
      y(y),
      z(z)
{

}

Quat::Quat(float angle, const vec3 & vec)
    : w(cos(angle / 2.f)),
      x(vec.x() * sin(angle / 2.f)),
      y(vec.y() * sin(angle / 2.f)),
      z(vec.z() * sin(angle / 2.f))
{
    normalize();
}

Quat::Quat(const Mat4f &mat)
    : w(sqrt(1.f + mat.data[0] + mat.data[5] + mat.data[10]) / 2.f)
{
    const float div = 1.f / (4.f * w);
    x = (mat.data[6] - mat.data[9]) * div;
    y = (mat.data[8] - mat.data[2]) * div;
    z = (mat.data[1] - mat.data[4]) * div;
    normalize();
}

Quat
Quat::add(const Quat &quat) const
{
    return Quat(this->w + quat.w, this->x + quat.x,
                this->y + quat.y, this->z + quat.z);
}

Quat
Quat::subtract(const Quat &quat) const
{
    return Quat(this->w - quat.w, this->x - quat.x,
                this->y - quat.y, this->z - quat.z);
}

Quat
Quat::multiply(const Quat &quat) const
{
    const float w = this->w;
    const float x = this->x;
    const float y = this->y;
    const float z = this->z;
    const float W = quat.w;
    const float X = quat.x;
    const float Y = quat.y;
    const float Z = quat.z;

    return Quat(w * W - x * X - y * Y - z * Z,
                w * X + x * W + y * Z - z * Y,
                w * Y - x * Z + y * W + z * X,
                w * Z + x * Y - y * X + z * W);
}

vec3
Quat::multiply(const vec3 &vec) const
{
    // nVidia SDK implementation
    vec3 uv, uuv;
    vec3 qvec(x, y, z);
    uv = cross(qvec, vec);
    uuv = cross(qvec, uv);
    uv *= (2.f * w);
    uuv *= 2.f;

    return vec + uv + uuv;
}

float
Quat::norm() const
{
    return sqrt(w * w + x * x + y * y + z * z);
}

Quat &
Quat::normalize()
{
    const float norm = this->norm();

    if (norm > 0.f)
    {
        w /= norm;
        x /= norm;
        y /= norm;
        z /= norm;
    }

    return *this;
}

Quat
Quat::operator+(const Quat &quat) const
{
    return this->add(quat);
}

Quat
Quat::operator-(const Quat &quat) const
{
    return this->subtract(quat);
}

Quat
Quat::operator*(const Quat &quat) const
{
    return this->multiply(quat);
}

vec3
Quat::operator*(const vec3 &vec) const
{
    return this->multiply(vec);
}

Quat & Quat::operator=(const Quat &quat)
{
    this->w = quat.w;
    this->x = quat.x;
    this->y = quat.y;
    this->z = quat.z;

    return *this;
}

bool
Quat::operator==(const Quat &quat) const
{
    return this->w == quat.w && this->x == quat.x &&
        this->y == quat.y && this->z == quat.z;
}

Quat
lerp(const Quat &initial, const Quat &final, float ratio)
{
    // TODO complete
    return Quat();
}

Quat
slerp(const Quat &initial, const Quat &final, float ratio)
{
    // TODO complete
    return Quat();
}
