#pragma once

namespace glm
{

template <typename T, int Size>
struct Vec_t
{
    T data[Size];
};

template <typename T>
struct Vec_t<T, 2>
{
    Vec_t() : x(0), y(0) {}
    Vec_t(T x, T y) : x(x), y(y) {}

    union
    {
        struct { T x, y; };
        struct { T u, v; };
        struct { T s, t; };

        T data[2];
    };
};

template <typename T>
struct Vec_t<T, 3>
{
    Vec_t() : x(0), y(0), z(0) {}
    Vec_t(T x, T y, T z) : x(x), y(y), z(z) {}

    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { Vec_t<T, 2> xy; };
        struct { Vec_t<T, 2> rg; };
        struct { T x_; Vec_t<T, 2> yz; };
        struct { T r_; Vec_t<T, 2> gb; };

        T data[3];
    };
};

template <typename T>
struct Vec_t<T, 4>
{
    Vec_t() : x(0), y(0), z(0), w(0) {}
    Vec_t(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    union
    {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { Vec_t<T, 2> xy; };
        struct { Vec_t<T, 2> rg; };
        struct { T x_; Vec_t<T, 2> yz; };
        struct { T r_; Vec_t<T, 2> gb; };
        struct { T x__; T y_; Vec_t<T, 2> zw; };
        struct { T r__; T g_; Vec_t<T, 2> ba; };
        Vec_t<T, 3> xyz;
        Vec_t<T, 3> rgb;

        T data[4];
    };
};

template <typename T, int size>
Vec_t<T, size> operator+(const Vec_t<T, size> &a,
                         const Vec_t<T, size> &b)
{
    Vec_t<T, size> res;

    for (int i = 0; i < size; ++i)
    {
        res.data[i] = a.data[i] + b.data[i];
    }

    return res;
}

template <typename T, int size>
Vec_t<T, size> operator-(const Vec_t<T, size> &a,
                         const Vec_t<T, size> &b)
{
    Vec_t<T, size> res;

    for (int i = 0; i < size; ++i)
    {
        res.data[i] = a.data[i] - b.data[i];
    }

    return res;
}

template <typename T, int size>
Vec_t<T, size> operator-(const Vec_t<T, size> &a)
{
    Vec_t<T, size> res;

    for (int i = 0; i < size; ++i)
    {
        res.data[i] = -a.data[i];
    }

    return res;
}

template <typename T, int size>
Vec_t<T, size> operator*(T value, const Vec_t<T, size> &vec)
{
    Vec_t<T, size> res;

    for (int i = 0; i < size; ++i)
    {
        res.data[i] = value * vec.data[i];
    }

    return res;
}

template <typename T, int size>
Vec_t<T, size> operator*(const Vec_t<T, size> &vec, T value)
{
    return operator*(value, vec);
}

template <typename T, int size>
Vec_t<T, size> operator/(const Vec_t<T, size> &vec, T value)
{
    const float recip = (T)1 / value;
    return operator*(recip, vec);
}

template <typename T, int size>
bool operator==(const Vec_t<T, size> &a, const Vec_t<T, size> &b)
{
    for (int i = 0; i < size; ++i)
    {
        if (a.data[i] != b.data[i])
        {
            return false;
        }
    }

    return true;
}

template <typename T, int size>
Vec_t<T, size> & operator+=(Vec_t<T, size> &a, const Vec_t<T, size> &b)
{
    for (int i = 0; i < size; ++i)
    {
        a.data[i] += b.data[i];
    }

    return a;
}

template <typename T, int size>
Vec_t<T, size> & operator-=(Vec_t<T, size> &a, const Vec_t<T, size> &b)
{
    for (int i = 0; i < size; ++i)
    {
        a.data[i] -= b.data[i];
    }

    return a;
}

template <typename T, int size>
Vec_t<T, size> & operator*=(Vec_t<T, size> &a, T value)
{
    for (int i = 0; i < size; ++i)
    {
        a.data[i] *= value;
    }
    return a;
}

template <typename T, int size>
Vec_t<T, size> & operator/=(Vec_t<T, size> &a, T value)
{
    const T recip = (T)1 / value;
    for (int i = 0; i < size; ++i)
    {
        a.data[i] *= recip;
    }
    return a;
}

template <typename T>
Vec_t<T, 3> cross(const Vec_t<T, 3> &a,
                  const Vec_t<T, 3> &b)
{
    Vec_t<T, 3> res;

    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;

    return res;
}

template <typename T, int size>
T dot(const Vec_t<T, size> &a,
      const Vec_t<T, size> &b)
{
    T res = (T)0;

    for (int i = 0; i < size; ++i)
    {
        res += a.data[i] * b.data[i];
    }

    return res;
}

template <typename T, int size>
T length2(const Vec_t<T, size> &vec)
{
    T res = 0.f;

    for (int i = 0; i < size; ++i)
    {
        res += vec.data[i] * vec.data[i];
    }

    return res;
}

template <typename T, int size>
T length(const Vec_t<T, size> &vec)
{
    return sqrt(length2(vec));
}

template <typename T, int size>
Vec_t<T, size> normalize(const Vec_t<T, size> &vec)
{
    Vec_t<T, size> res = vec;
    res /= length(res);
    return res;
}

template <typename T, int size>
void print(const Vec_t<T, size> &vec)
{
    printf("\n(");
    for (int i = 0; i < (size - 1); ++i)
    {
        printf("%.2f, ", vec.data[i]);
    }
    printf("%.2f)\n", vec.data[size - 1]);
}

typedef Vec_t<float, 2> Vec2;
typedef Vec_t<float, 3> Vec3;
typedef Vec_t<float, 4> Vec4;

typedef Vec_t<double, 2> Vec2d;
typedef Vec_t<double, 3> Vec3d;
typedef Vec_t<double, 4> Vec4d;

typedef Vec_t<int, 2> Vec2i;
typedef Vec_t<int, 3> Vec3i;
typedef Vec_t<int, 4> Vec4i;

}
