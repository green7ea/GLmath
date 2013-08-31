#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "vec.h"
// #include "mat4.h"

template <typename Type>
class quat_t
{
public:

    quat_t(Type w = 1.f, Type x = 0.f, Type y = 0.f, Type z = 0.f)
        : w(w),
          x(x),
          y(y),
          z(z)
    {

    }

    quat_t(Type angle, const vec_t<Type, 3> &vec)
        : w(cos(angle / 2.f)),
          x(vec.x() * sin(angle / 2.f)),
          y(vec.y() * sin(angle / 2.f)),
          z(vec.z() * sin(angle / 2.f))
    {
        normalize();
    }

    quat_t(const vec3 &vec_a,
           const vec3 &vec_b)
    {
        const vec3 vec1 = norm(vec_a);
        const vec3 vec2 = norm(vec_b);
        const float dot_res = dot(vec1, vec2);
        if (dot_res >= 0.999 ||
            dot_res <= -0.999) // TODO we should flip the vector by 180
        {
            w = 1;
            x = 0;
            y = 0;
            z = 0;
        }

        const vec3 a = cross(vec1, vec2);
        x = a.x();
        y = a.y();
        z = a.z();
        w = 1 + dot_res;
        normalize();
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

    inline Type
    length() const
    {
        return sqrt(w * w + x * x + y * y + z * z);
    }

    inline quat_t<Type> &
    normalize()
    {
        const Type norm = this->length();

        if (norm > 0)
        {
            const Type recip = 1 / norm;
            w *= recip;
            x *= recip;
            y *= recip;
            z *= recip;
        }

        return *this;
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
        const float w = this->w;
        const float x = this->x;
        const float y = this->y;
        const float z = this->z;
        const float W = quat.w;
        const float X = quat.x;
        const float Y = quat.y;
        const float Z = quat.z;

        return quat_t<Type>(w * W - x * X - y * Y - z * Z,
                            w * X + x * W + y * Z - z * Y,
                            w * Y - x * Z + y * W + z * X,
                            w * Z + x * Y - y * X + z * W);
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
inline quat_t<Type>
lerp(const quat_t<Type> &initial, const quat_t<Type> &final,
     float ratio)
{
    // TODO complete
    return quat_t<Type>();
}

template <typename Type>
inline quat_t<Type>
slerp(const quat_t<Type> &initial, const quat_t<Type> &final,
      float ratio)
{
    // TODO complete
    return quat_t<Type>();
}

typedef quat_t<float> quat;
typedef quat_t<double> quatd;

#endif // __QUATERNION_H__
