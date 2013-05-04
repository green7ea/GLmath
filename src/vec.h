#ifndef __VEC_H__
#define __VEC_H__

#include <cmath>
#include <cstdint>
#include <cassert>

template <typename Type, int Size>
class vec_t
{
public:

    vec_t(Type x = 0, Type y = 0, Type z = 0, Type w = 0)
    {
        if (Size >= 1)
        {
            data[0] = x;
        }
        if (Size >= 2)
        {
            data[1] = y;
        }
        if (Size >= 3)
        {
            data[2] = z;
        }
        if (Size == 4)
        {
            data[3] = w;
        }
        else if (Size > 4)
        {
            assert(true);
        }
    }

    vec_t(std::initializer_list<Type> values)
    {
        int i = 0;
        for (const Type value: values)
        {
            if (i >= Size) break;

            data[i++] = value;
        }
        for (; i < Size; ++i)
        {
            data[i] = 0;
        }
    }

    inline Type &
    operator[](uint8_t index)
    {
        if (Size >= (index + 1))
        {
            return data[index];
        }
        else
        {
            assert(true);
            // To suppress warning
            return data[0];
        }
    }

    inline Type
    operator[](uint8_t index) const
    {
        if (Size >= (index + 1))
        {
            return data[index];
        }
        else
        {
            return 0;
        }
    }

    inline vec_t<Type, Size>
    operator+(const vec_t<Type, Size> &vec) const
    {
        return vec_t<Type, Size>((*this)[0] + vec[0], (*this)[1] + vec[1],
                                 (*this)[2] + vec[2], (*this)[3] + vec[3]);
    }

    inline void
    operator+=(const vec_t<Type, Size> &vec)
    {
        for (uint8_t i = 0; i < Size; ++i)
        {
            (*this)[i] += vec[i];
        }
    }

    inline vec_t<Type, Size>
    operator-(const vec_t<Type, Size> &vec) const
    {
        return vec_t<Type, Size>((*this)[0] - vec[0], (*this)[1] - vec[1],
                                 (*this)[2] - vec[2], (*this)[3] - vec[3]);
    }

    inline void
    operator-=(const vec_t<Type, Size> &vec)
    {
        for (uint8_t i = 0; i < Size; ++i)
        {
            (*this)[i] -= vec[i];
        }
    }

    inline vec_t<Type, Size>
    operator*(Type value) const
    {
        return vec_t(value * (*this)[0], value * (*this)[1],
                     value * (*this)[2], value * (*this)[3]);
    }

    inline void
    operator*=(Type value)
    {
        for (uint8_t i = 0; i < Size; ++i)
        {
            (*this)[i] *= value;
        }
    }

    vec_t<Type, Size>
    operator/(Type value) const
    {
        const float inv = 1.f / value;
        return vec_t(inv * (*this)[0], inv * (*this)[1],
                     inv * (*this)[2], inv * (*this)[3]);
    }

    void
    operator/=(Type value)
    {
        const float newVal = 1.f / value;
        for (uint8_t i = 0; i < Size; ++i)
        {
            (*this)[i] *= newVal;
        }
    }

    vec_t<Type, Size> &
    operator=(const vec_t<Type, Size> &vec)
    {
        for (uint8_t i = 0; i < Size; ++i)
        {
            (*this)[i] = vec[i];
        }
        return *this;
    }

    bool
    operator==(const vec_t<Type, Size> &vec) const
    {
        return vec[0] == vec[0] && vec[1] == vec[1] &&
            vec[2] == vec[2] && vec[3] == vec[3];
    }

    vec_t<Type, 4>
    xyzw() const
    {
        return vec_t<Type, 4>((*this)[0], (*this)[1], (*this)[2], (*this)[3]);
    }

    vec_t<Type, 3>
    xyz() const
    {
        return vec_t<Type, 3>((*this)[0], (*this)[1], (*this)[2]);
    }

    vec_t<Type, 3>
    xzy() const
    {
        return vec_t<Type, 3>((*this)[0], (*this)[2], (*this)[1]);
    }

    vec_t<Type, 3>
    yxz() const
    {
        return vec_t<Type, 3>((*this)[1], (*this)[0], (*this)[2]);
    }

    vec_t<Type, 3>
    yzx() const
    {
        return vec_t<Type, 3>((*this)[1], (*this)[2], (*this)[0]);
    }

    vec_t<Type, 3>
    zxy() const
    {
        return vec_t<Type, 3>((*this)[2], (*this)[0], (*this)[1]);
    }

    vec_t<Type, 3>
    zyx() const
    {
        return vec_t<Type, 3>((*this)[2], (*this)[1], (*this)[0]);
    }

    vec_t<Type, 2>
    xy() const
    {
        return vec_t<Type, 2>((*this)[0], (*this)[1]);
    }

    vec_t<Type, 2>
    yx() const
    {
        return vec_t<Type, 2>((*this)[1], (*this)[0]);
    }

    vec_t<Type, 2>
    zx() const
    {
        return vec_t<Type, 2>((*this)[2], (*this)[0]);
    }

    vec_t<Type, 2>
    zy() const
    {
        return vec_t<Type, 2>((*this)[2], (*this)[1]);
    }

    vec_t<Type, 2>
    xz() const
    {
        return vec_t<Type, 2>((*this)[0], (*this)[2]);
    }

    vec_t<Type, 2>
    yz() const
    {
        return vec_t<Type, 2>((*this)[1], (*this)[2]);
    }

    Type &
    x()
    {
        return (*this)[0];
    }

    Type &
    y()
    {
        return (*this)[1];
    }

    Type &
    z()
    {
        return (*this)[2];
    }

    Type &
    w()
    {
        return (*this)[3];
    }

    Type
    x() const
    {
        return (*this)[0];
    }

    Type
    y() const
    {
        return (*this)[1];
    }

    Type
    z() const
    {
        return (*this)[2];
    }

    Type
    w() const
    {
        return (*this)[3];
    }

private:
    Type data[Size];
};

template <typename Type, int Size>
vec_t<Type, Size>
operator*(Type value, const vec_t<Type, Size> &vec)
{
    return vec * value;
}

template <typename Type, int Size>
float
length(const vec_t<Type, Size> &vec)
{
    float sum = 0.f;
    for (uint8_t i = 0; i < Size; ++i)
    {
        sum += (float)(vec[i] * vec[i]);
    }
    return sqrt(sum);
}

template <typename Type, int Size>
float
distance(const vec_t<Type, Size> &v1, const vec_t<Type, Size> &v2)
{
    return length(v2 - v1);
}

template <typename Type, int Size>
Type
dot(const vec_t<Type, Size> &v1, const vec_t<Type, Size> &v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

template <typename Type, int Size>
vec_t<Type, Size>
cross(const vec_t<Type, Size> &v1, const vec_t<Type, Size> &v2)
{
    // TODO this is for 2 or 3 vectors not 4
    return vec_t<Type, Size>(v1[1] * v2[2] - v2[1] * v1[2],
                             v2[0] * v1[2] - v1[0] * v2[2],
                             v1[0] * v2[1] - v2[0] * v1[1]);
}

template <typename Type, int Size>
vec_t<Type, Size>
normalize(const vec_t<Type, Size> &vec)
{
    return vec / length(vec);
}

typedef vec_t<float, 4> vec4;
typedef vec_t<double, 4> dvec4;
typedef vec_t<int32_t, 4> ivec4;
typedef vec_t<uint32_t, 4> uvec4;

typedef vec_t<float, 3> vec3;
typedef vec_t<double, 3> dvec3;
typedef vec_t<int32_t, 3> ivec3;
typedef vec_t<uint32_t, 3> uvec3;

typedef vec_t<float, 2> vec2;
typedef vec_t<double, 2> dvec2;
typedef vec_t<int32_t, 2> ivec2;
typedef vec_t<uint32_t, 2> uvec2;

#endif // __VEC_H__
