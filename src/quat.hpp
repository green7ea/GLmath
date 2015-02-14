#pragma once

#include "vec.h"

template <typename T>
struct Quat_t
{
    T w;
    T x;
    T y;
    T z;
};

template <typename T>
Quat_t<T> quat_from_axis_angle(T angle, const Vec_t<T, 3> &vec)
{
    return
    {
        .w = cos(angle * (T)0.5),
        .x = vec.x() * sin(angle * (T)0.5),
        .y = vec.y() * sin(angle * (T)0.5),
        .z = vec.z() * sin(angle * (T)0.5)
    };
}

template <typename T>
Quat_t<T> quat_from_euler(T yaw, T pitch, T roll)
{
    const T cos_phi = cos(roll * (T)0.5);
    const T sin_phi = sin(roll * (T)0.5);

    const T cos_theta = cos(pitch * (T)0.5);
    const T sin_theta = sin(pitch * (T)0.5);

    const T cos_psi = cos(yaw * (T)0.5);
    const T sin_psi = cos(yaw * (T)0.5);

    return
    {
        .w = cos_phi * cos_theta * sin_psi - sin_phi * sin_theta * cos_psi,
        .x = cos_phi * cos_theta * cos_psi + sin_phi * sin_theta * sin_psi,
        .y = sin_phi * cos_theta * cos_psi - cos_phi * sin_theta * sin_psi,
        .z = cos_phi * sin_theta * cos_psi + sin_phi * cos_theta * sin_psi
    };
}

template <typename T>
T length(const Quat_t<T> &quat)
{
    return sqrt(quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
}

template <typename T>
Quat_t<T> norm(const Quat_t<T> &quat)
{
    const T len = length(quat);

    if (len > 0.f)
    {
        return Quat_t<T>(len * quat.w,
                            len * quat.x,
                            len * quat.y,
                            len * quat.z);
    }
    return Quat_t<T>(1.f, 0.f, 0.f, 0.f);
}

template <typename T>
Quat_t<T> make_quat(const Vec_t<T, 3> &a, const Vec_t<T, 3> &b)
{
    a = norm(a);
    b = norm(b);
    const T dot_res = dot(a, b);
    if (dot_res >= 0.999 ||
        dot_res <= -0.999) // TODO we should flip the vector by 180
    {
        return
        {
            .w = 1,
            .x = 0,
            .y = 0,
            .z = 0
        };
    }

    const Vec_t<T, 3> c = cross(a, b);
    return
    {
        .w = (T)1 + dot_res,
        .x = c.x(),
        .y = c.y(),
        .z = c.z()
    };
}

template <typename T>
Quat_t<T> operator+(const Quat_t<T> &a, const Quat_t<T> &b)
{
    return
    {
        .w = a.w + b.w,
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
}

template <typename T>
Quat_t<T> operator-(const Quat_t<T> &a, const Quat_t<T> &b)
{
    return
    {
        .w = a.w - b.w,
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
}

template <typename T>
Quat_t<T> operator*(const Quat_t<T> &a, const Quat_t<T> &b)
{
    return { a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
             a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
             a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
             a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w };
}

template <typename T>
Vec_t<T, 3> operator*(const Quat_t<T> &quat, const Vec_t<T, 3> &vec)
{
    Vec_t<T, 3> uv, uuv;
    Vec_t<T, 3> qvec(quat.x, quat.y, quat.z);
    uv = cross(qvec, vec);
    uuv = cross(qvec, uv);
    uv *= (2.f * quat.w);
    uuv *= 2.f;

    return vec + uv + uuv;
}

template <typename T>
bool operator==(const Quat_t<T> &a, const Quat_t<T> &b)
{
    return (a.w == b.w && a.x == b.x && a.y == b.y && a.z == b.z);
}

template <typename T>
Quat_t<T> lerp(const Quat_t<T> &, const Quat_t<T> &, float)
{
    // TODO complete
    return {};
}

template <typename T>
Quat_t<T> slerp(const Quat_t<T> &, const Quat_t<T> &, float)
{
    // TODO complete
    return {};
}

typedef Quat_t<float> Quat;
typedef Quat_t<double> Quatd;
