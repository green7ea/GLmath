#pragma once

#include <cstdio>
#include <cassert>
#include <initializer_list>

#include "vec.hpp"

/*
  These matrices are all column major. The best way to think about
  them is to see them as an array of column vectors.

  / 0  4  8 12 \     / m00 m01 m02 m03 \
  | 1  5  9 13 |  =  | m10 m11 m12 m13 |
  | 2  6 10 14 |  =  | m20 m21 m22 m23 |
  \ 3  7 11 15 /     \ m30 m31 m32 m33 /
 */

namespace glm
{

template <typename T, int size>
struct Mat_t
{
    Mat_t()
    {
        for (int i = 0; i < (size * size); ++i)
        {
            data[i] = (i % (size + 1)) == 0 ? 1.f : 0.f;
        }
    }

    Mat_t(const std::initializer_list<T> &list)
    {
        const int list_size = list.size();
        assert(list_size == size * size);

        int i = 0;
        for (const auto &item : list)
        {
            const int row = i / size;
            const int col = i % size;
            const int tranposed_i = row + col * size;
            data[tranposed_i] = item;
            ++i;
        }
    }

    Mat_t & operator=(const Mat_t<T, size> &b)
    {
        for (int i = 0; i < size * size; ++i)
        {
            data[i] = b.data[i];
        }

        return *this;
    }

    union
    {
        T data[size * size];
    };
};

template <typename T>
struct Mat_t<T, 3>
{
    Mat_t()
    {
        for (int i = 0; i < 9; ++i)
        {
            data[i] = (i % 4) == 0 ? 1.f : 0.f;
        }
    }

    Mat_t(const std::initializer_list<T> &list)
    {
        const int list_size = list.size();
        assert(list_size == 9);

        int i = 0;
        for (const auto &item : list)
        {
            data[i] = item;
            ++i;
        }
    }

    Mat_t & operator=(const Mat_t<T, 3> &b)
    {
        for (int i = 0; i < 9; ++i)
        {
            data[i] = b.data[i];
        }

        return *this;
    }

    union
    {
        struct
        {
            T m00, m10, m20;
            T m01, m11, m21;
            T m02, m12, m22;
        };

        T data[9];
    };
};

template <typename T>
struct Mat_t<T, 4>
{
    Mat_t()
    {
        for (int i = 0; i < 16; ++i)
        {
            data[i] = (i % 5) == 0 ? 1.f : 0.f;
        }
    }

    Mat_t(const std::initializer_list<T> &list)
    {
        const int list_size = list.size();
        assert(list_size == 16);

        int i = 0;
        for (const auto &item : list)
        {
            const int row = i / 4;
            const int col = i % 4;
            const int tranposed_i = row + (col * 4);
            data[tranposed_i] = item;
            ++i;
        }
    }

    Mat_t & operator=(const Mat_t<T, 4> &b)
    {
        for (int i = 0; i < 16; ++i)
        {
            data[i] = b.data[i];
        }

        return *this;
    }

    union
    {
        struct
        {
            T m00, m10, m20, m30;
            T m01, m11, m21, m31;
            T m02, m12, m22, m32;
            T m03, m13, m23, m33;
        };

        struct
        {
            Vec_t<T, 3> base_a;
            T m03_;
            Vec_t<T, 3> base_b;
            T m13_;
            Vec_t<T, 3> base_c;
            T m23_;
            Vec_t<T, 3> translation;
            T m33_;
        };

        T data[16];
    };
};

template <typename T>
Mat_t<T, 3> matrix_from_axis_angle(const Vec_t<T, 3> &axis, T angle)
{
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/
    const T c = cos(angle);
    const T s = sin(angle);
    const T t = 1 - c;
    const Vec_t<T, 3> na = axis / length(axis);
    const T x = na.x();
    const T y = na.y();
    const T z = na.z();

    Mat_t<T, 3> res = {
        t * x * x + c,
        t * x * y + z * s,
        t * x * z - y * s,
        t * x * y - z * s,
        t * y * y + c,
        t * y * z + x * s,
        t * x * z + y * s,
        t * y * z - x * s,
        t * z * z + c };

    return res;
}

// TODO make this size agnostic
template <typename T>
Vec_t<T, 4> operator*(const Mat_t<T, 4> &mat, const Vec_t<T, 4> &vec)
{
    return Vec_t<T, 4>(mat.data[0] * vec.x + mat.data[4] * vec.y +
                       mat.data[8] * vec.z + mat.data[12] * vec.w,
                       mat.data[1] * vec.x + mat.data[5] * vec.y +
                       mat.data[9] * vec.z + mat.data[13] * vec.w,
                       mat.data[2] * vec.x + mat.data[6] * vec.y +
                       mat.data[10] * vec.z + mat.data[14] * vec.w,
                       mat.data[3] * vec.x + mat.data[7] * vec.y +
                       mat.data[11] * vec.z + mat.data[15] * vec.w);
}

template <typename T, int size>
Mat_t<T, size> operator*(const Mat_t<T, size> &a, const Mat_t<T, size> &b)
{
    Mat_t<T, size> result;
    T *res = result.data;
    const T *row, *column;

    const int size2 = size * size;
    for (int i = 0; i < size2; ++i)
    {
        row = &a.data[i % size];
        column = &b.data[size * (i / size)];
        res[i] = 0;
        for (int j = 0; j < size; ++j)
        {
            res[i] += row[size * j] * column[j];
        }
    }

    return result;
}

template <typename T, int size>
bool operator==(const Mat_t<T, size> &a, const Mat_t<T, size> &b)
{
    const int msize = size * size;
    for (int i = 0; i < msize; ++i) {
        if (a.data[i] != b.data[i])
        {
            return false;
        }
    }

    return true;
}

template <typename T>
Mat_t<T, 4> inverse(const Mat_t<T, 4> &matrix)
{
    const T *mat = matrix.data;
    Mat_t<T, 4> res;
    T *dst = res.data;
    T tmp[12];
    T src[16];
    T det;

    for (int i = 0; i < 4; ++i)
    {
        src[i] = mat[i * 4];
        src[i + 4] = mat[i * 4 + 1];
        src[i + 8] = mat[i * 4 + 2];
        src[i + 12] = mat[i * 4 + 3];
    }

    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];

    dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
    dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
    dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
    dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
    dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
    dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
    dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
    dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
    dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
    dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
    dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
    dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
    dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
    dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
    dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
    dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

    tmp[0] = src[2] * src[7];
    tmp[1] = src[3] * src[6];
    tmp[2] = src[1] * src[7];
    tmp[3] = src[3] * src[5];
    tmp[4] = src[1] * src[6];
    tmp[5] = src[2] * src[5];
    tmp[6] = src[0] * src[7];
    tmp[7] = src[3] * src[4];
    tmp[8] = src[0] * src[6];
    tmp[9] = src[2] * src[4];
    tmp[10] = src[0] * src[5];
    tmp[11] = src[1] * src[4];

    dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
    dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
    dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
    dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
    dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
    dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
    dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
    dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
    dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
    dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
    dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
    dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
    dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
    dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
    dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
    dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

    det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];
    det = 1.f / det;

    for (int i = 0; i < 16; ++i)
    {
        dst[i] *= det;
    }

    return Mat_t<T, 4>(dst);
}

template <typename T, int size>
Mat_t<T, size> transpose(const Mat_t<T, size> &matrix)
{
    Mat_t<T, size> res;

    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            res.data[x + y * size] = matrix.data[x * size + y];
        }
    }

    return res;
}

template <typename T>
Mat_t<T, 4> projection(T left, T right, T bottom, T top, T near, T far)
{
    Mat_t<T, 4> mat;

    mat.m00 = (2 * near) / (right - left);
    mat.m10 = 0;
    mat.m20 = 0;
    mat.m30 = 0;

    mat.m01 = 0;
    mat.m11 = (2 * near) / (top - bottom);
    mat.m21 = 0;
    mat.m31 = 0;

    mat.m02 = (right + left) / (right - left);
    mat.m12 = (top + bottom) / (top - bottom);
    mat.m22 = (far + near) / (far - near);
    mat.m32 = 1;

    mat.m03 = 0;
    mat.m13 = 0;
    mat.m23 = (-2 * far * near) / (far - near);
    mat.m33 = 0;

    return mat;
}

template <typename Type>
Mat_t<Type, 4> lookat(const Vec_t<Type, 3> &position,
                      const Vec_t<Type, 3> &point,
                      const Vec_t<Type, 3> &up)
{
    Mat_t<Type, 4> mat;

    const auto forward = normalize(point - position);

    const auto left = normalize(cross(up, forward));
    mat.m00 = left.x;
    mat.m01 = left.y;
    mat.m02 = left.z;

    const auto normed_up = normalize(cross(forward, left));
    mat.m10 = normed_up.x;
    mat.m11 = normed_up.y;
    mat.m12 = normed_up.z;

    mat.m20 = forward.x;
    mat.m21 = forward.y;
    mat.m22 = forward.z;

    mat.m03 = position.x;
    mat.m13 = position.y;
    mat.m23 = position.z;

    mat.m30 = 0.f;
    mat.m31 = 0.f;
    mat.m32 = 0.f;
    mat.m33 = 1.f;

    return mat;
}

template <typename T, int size>
void print(const Mat_t<T, size> &mat)
{
    printf("\n");

    for (int row = 0; row < size; ++row)
    {
        printf("|  ");
        for (int col = 0; col < size; ++col)
        {
            printf("%.2f  ", (float) mat.data[col * size + row]);
        }
        printf("|\n");
    }
}

typedef Mat_t<float, 3> Mat3;
typedef Mat_t<float, 4> Mat4;

typedef Mat_t<double, 3> Mat3d;
typedef Mat_t<double, 4> Mat4d;

}
