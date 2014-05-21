#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "vec.h"
// #include "mat4.h"

template <typename Type>
class quat_t
{
public:

    quat_t(Type quat_w = 1.f, Type quat_x = 0.f, Type quat_y = 0.f, Type quat_z = 0.f)
        : w(quat_w),
          x(quat_x),
          y(quat_y),
          z(quat_z)
    {

    }

    quat_t(Type angle, const vec_t<Type, 3> &vec)
        : w(cos(angle / 2.f)),
          x(vec.x() * sin(angle / 2.f)),
          y(vec.y() * sin(angle / 2.f)),
          z(vec.z() * sin(angle / 2.f))
    {

    }

    quat_t(const vec3 &vec_a,
           const vec3 &vec_b)
    {
        const vec3 vec_1 = norm(vec_a);
        const vec3 vec_2 = norm(vec_b);
        const float dot_res = dot(vec_1, vec_2);
        if (dot_res >= 0.999 ||
            dot_res <= -0.999) // TODO we should flip the vector by 180
        {
            w = 1;
            x = 0;
            y = 0;
            z = 0;
        }

        const vec3 a = cross(vec_1, vec_2);
        x = a.x();
        y = a.y();
        z = a.z();
        w = 1 + dot_res;
    }

    quat_t(std::initializer_list<Type> values)
    {
        int i = 0;
        for (const Type value: values)
        {
            if (i >= 4) break;

            switch (i)
            {
            case 0:
                w = value;
                break;
            case 1:
                x = value;
                break;
            case 2:
                y = value;
                break;
            case 3:
                z = value;
                break;
            default:
                assert(false);
                break;
            }
        }
    }

    inline quat_t<Type>
    operator+(const quat_t<Type> &quat) const
    {
        return quat_t<Type>(this->w + quat.w, this->x + quat.x,
                            this->y + quat.y, this->z + quat.z);
    }

    inline quat_t<Type>
    operator-(const quat_t<Type> &quat) const
    {
        return quat_t<Type>(this->w - quat.w, this->x - quat.x,
                            this->y - quat.y, this->z - quat.z);
    }

    inline quat_t<Type>
    operator*(const quat_t<Type> &quat) const
    {
        const float w_1 = this->w;
        const float x_1 = this->x;
        const float y_1 = this->y;
        const float z_1 = this->z;
        const float W_1 = quat.w;
        const float X_1 = quat.x;
        const float Y_1 = quat.y;
        const float Z_1 = quat.z;

        return quat_t<Type>(w_1 * W_1 - x_1 * X_1 - y_1 * Y_1 - z_1 * Z_1,
                            w_1 * X_1 + x_1 * W_1 + y_1 * Z_1 - z_1 * Y_1,
                            w_1 * Y_1 - x_1 * Z_1 + y_1 * W_1 + z_1 * X_1,
                            w_1 * Z_1 + x_1 * Y_1 - y_1 * X_1 + z_1 * W_1);
    }

    inline vec_t<Type, 3>
    operator*(const vec_t<Type, 3> &vec) const
    {
        vec_t<Type, 3> uv, uuv;
        vec_t<Type, 3> qvec(x, y, z);
        uv = cross(qvec, vec);
        uuv = cross(qvec, uv);
        uv *= (2.f * w);
        uuv *= 2.f;

        return vec + uv + uuv;
    }

    inline quat_t<Type> &
    operator=(const quat_t<Type> &quat)
    {
        this->w = quat.w;
        this->x = quat.x;
        this->y = quat.y;
        this->z = quat.z;

        return *this;
    }

    inline bool
    operator==(const quat_t<Type> &quat) const
    {
        return (this->w == quat.w && this->x == quat.x &&
                this->y == quat.y && this->z == quat.z);
    }

    Type w;
    Type x;
    Type y;
    Type z;
};

template <typename Type>
Type length(const quat_t<Type> &quat)
{
    return sqrt(quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
}

template <typename Type>
quat_t<Type> norm(const quat_t<Type> &quat)
{
    const Type len = length(quat);

    if (len > 0.f)
    {
        return quat_t<Type>(len * quat.w,
                            len * quat.x,
                            len * quat.y,
                            len * quat.z);
    }
    return quat_t<Type>(1.f, 0.f, 0.f, 0.f);
}

template <typename Type>
inline quat_t<Type>
lerp(const quat_t<Type> &, const quat_t<Type> &, float)
{
    // TODO complete
    return quat_t<Type>();
}

template <typename Type>
inline quat_t<Type>
slerp(const quat_t<Type> &, const quat_t<Type> &, float)
{
    // TODO complete
    return quat_t<Type>();
}

typedef quat_t<float> quat;
typedef quat_t<double> quatd;

#endif // __QUATERNION_H__
