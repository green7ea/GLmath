#pragma once

template <typename Type, int Size>
union Vec_t
{
    Type v[Size];
};

template <typename Type>
union Vec_t<Type, 2>
{
    struct { Type x, y; };

    Type v[2];
};

template <typename Type>
union Vec_t<Type, 3>
{
    struct { Type x, y, z; };
    struct { Vec_t<Type, 2> xy; };
    struct { Type x_; Vec_t<Type, 2> yz; };

    Type v[3];
};

template <typename Type>
union Vec_t<Type, 4>
{
    struct { Type x, y, z, w; };
    struct { Vec_t<Type, 2> xy; };
    struct { Type x_; Vec_t<Type, 2> yz; };
    struct { Type x__; Type y_; Vec_t<Type, 2> zw; };
    Vec_t<Type, 3> xyz;

    Type v[4];
};

template <typename T, int size>
Vec_t<T, size> operator+(const Vec_t<T, size> &a,
                         const Vec_t<T, size> &b)
{
    Vec_t<T, size> res;

    for (int i = 0; i < size; ++i)
    {
        res.v[i] = a.v[i] + b.v[i];
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
        res.v[i] = a.v[i] - b.v[i];
    }

    return res;
}

template <typename T, int size>
Vec_t<T, size> operator-(const Vec_t<T, size> &a)
{
    Vec_t<T, size> res;

    for (int i = 0; i < size; ++i)
    {
        res.v[i] = -a.v[i];
    }

    return res;
}

template <typename T, int size>
Vec_t<T, size> operator*(T value, const Vec_t<T, size> &vec)
{
    Vec_t<T, size> res;

    for (int i = 0; i < size; ++i)
    {
        res.v[i] = value * vec.v[i];
    }
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
        if (a.v[i] != b.v[i])
        {
            return false;
        }
    }
    
    return true;
}

template <typename T, int size>
Vec_t<T, size> & operator*=(Vec_t<T, size> &a, T value)
{
    a = a * value;
    return a;
}

template <typename T, int size>
Vec_t<T, size> & operator/=(Vec_t<T, size> &a, T value)
{
    a = a / value;
    return a;
}

template <typename T, int size>
Vec_t<T, size> cross(const Vec_t<T, size> &a,
                     const Vec_t<T, size> &b)
{
    Vec_t<T, size> res;

    // TODO complete this

    return res;
}

template <typename T, int size>
T dot(const Vec_t<T, size> &a,
      const Vec_t<T, size> &b)
{
    T res = (T)0;

    for (int i = 0; i < size; ++i)
    {
        res += a.v[i] * b.v[i];
    }

    return res;
}

template <typename T, int size>
T length2(const Vec_t<T, size> &vec)
{
    T res = 0.f;

    for (int i = 0; i < size; ++i)
    {
        res += vec.v[i] * vec.v[i];
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
