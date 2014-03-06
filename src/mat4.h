#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <cstring>
#include <string>
#include <initializer_list>

#include "vec.h"
#include "quat.h"

/*
  These matrices are all column major. The best way to think about
  them is to see them as an array of column vectors.

  / 0  4  8 12 \
  | 1  5  9 13 |
  | 2  6 10 14 |
  \ 3  7 11 15 /
 */

template <typename Type>
class mat4_t
{
public:

    mat4_t()
    {
        for (int i = 0; i < 16; ++i)
        {
            (i == 0 || i == 5 || i == 10 || i == 15) ?
                data[i] = 1:
                data[i] = 0;
        }
    }

    mat4_t(const Type *n)
    {
        memcpy(data, n, 16 * sizeof(Type));
    }

    mat4_t(std::initializer_list<Type> values)
    {
        int i = 0;
        for (const Type value: values)
        {
            if (i >= 16) break;

            data[i++] = value;
        }
        for (; i < 16; ++i)
        {
            (i == 0 ||
             i == 5 ||
             i == 10 ||
             i == 15) ?
                data[i] = 1 :
                data[i] = 0;
        }
    }

    mat4_t(const mat4_t &n)
    {
        operator=(n);
    }

    mat4_t(mat4_t &&n)
    {
        this->data = n.data;
        n.data = NULL;
    }

	mat4_t(const vec_t<Type, 3> &base_x,
           const vec_t<Type, 3> &base_y,
           const vec_t<Type, 3> &base_z)
    {
        data[0] = base_x.x();
        data[1] = base_y.x();
        data[2] = base_z.x();
        data[3] = 0.f;
        data[4] = base_x.y();
        data[5] = base_y.y();
        data[6] = base_z.y();
        data[7] = 0.f;
        data[8] = base_x.z();
        data[9] = base_y.z();
        data[10] = base_z.z();
        data[11] = 0.f;
        data[12] = 0.f;
        data[13] = 0.f;
        data[14] = 0.f;
        data[15] = 1.f;
    }

    mat4_t(const quat_t<Type> &q, const vec_t<Type, 3> &trans)
    {
        // Rotation
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/
        data[0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
        data[1] = 2 * q.x * q.y + 2 * q.z * q.w;
        data[2] = 2 * q.x * q.z - 2 * q.y * q.w;
        data[3] = 0;
        data[4] = 2 * q.x * q.y - 2 * q.z * q.w;
        data[5] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
        data[6] = 2 * q.y * q.z + 2 * q.x * q.w;
        data[7] = 0;
        data[8] = 2 * q.x * q.z + 2 * q.y * q.w;
        data[9] = 2 * q.y * q.z - 2 * q.x * q.w;
        data[10] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
        data[11] = 0;

        // Translation
        data[12] = trans.x();
        data[13] = trans.y();
        data[14] = trans.z();
        data[15] = 1;
        
    }

    mat4_t(const vec_t<Type, 3> &axis, Type angle)
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/
        const Type c = cos(angle);
        const Type s = sin(angle);
        const Type t = 1 - c;
        const vec_t<Type, 3> na = axis / length(axis);
        const Type x = na.x();
        const Type y = na.y();
        const Type z = na.z();

        data[0] = t * x * x + c;
        data[1] = t * x * y + z * s;
        data[2] = t * x * z - y * s;
        data[3] = 0;
        data[4] = t * x * y - z * s;
        data[5] = t * y * y + c;
        data[6] = t * y * z + x * s;
        data[7] = 0;
        data[8] = t * x * z + y * s;
        data[9] = t * y * z - x * s;
        data[10] = t * z * z + c;
        data[11] = 0;
        data[12] = 0;
        data[13] = 0;
        data[14] = 0;
        data[15] = 1;
    }

    ~mat4_t()
    {
        if (data)
        {
            delete [] data;
            data = NULL;
        }
    }

    inline mat4_t<Type>
    inverse() const
    {
        const Type *mat = this->data;
        Type dst[16];
        Type tmp[12];
        Type src[16];
        Type det;

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

        return mat4_t<Type>(dst);
    }

    inline mat4_t<Type>
    transpose() const
    {
        return mat4_t<Type>({ data[0], data[4], data[8], data[12],
                    data[1], data[5], data[9], data[13],
                    data[2], data[6], data[10], data[14],
                    data[3], data[7], data[11], data[15] });
    }

    inline mat4_t<Type>
    operator+(const mat4_t<Type> &mat) const
    {
        Type ldata[16];

        for (int i = 0; i < 16; ++i)
        {
            ldata[i] = this->data[i] + mat.data[i];
        }

        return mat4_t<Type>(ldata);
    }

    inline mat4_t<Type>
    operator-(const mat4_t<Type> &mat) const
    {
        Type ldata[16];

        for (int i = 0; i < 16; ++i)
        {
            ldata[i] = this->data[i] - mat.data[i];
        }

        return mat4_t<Type>(ldata);
    }

    inline vec_t<Type, 4>
    operator*(const vec_t<Type, 4> &vec) const
    {
        return vec_t<Type, 4>(data[0] * vec.x() + data[4] * vec.y() +
                              data[8] * vec.z() + data[12] * vec.w(),
                              data[1] * vec.x() + data[5] * vec.y() +
                              data[9] * vec.z() + data[13] * vec.w(),
                              data[2] * vec.x() + data[6] * vec.y() +
                              data[10] * vec.z() + data[14] * vec.w(),
                              data[3] * vec.x() + data[7] * vec.y() +
                              data[11] * vec.z() + data[15] * vec.w());
    }

    inline vec_t<Type, 3>
    operator*(const vec_t<Type, 3> &vec) const
    {
        return vec_t<Type, 3>(data[0] * vec.x() + data[4] * vec.y() + data[8] * vec.z() + data[12],
                              data[1] * vec.x() + data[5] * vec.y() + data[9] * vec.z() + data[13],
                              data[2] * vec.x() + data[6] * vec.y() + data[10] * vec.z() + data[14]);
    }

    inline mat4_t<Type>
    operator*(const mat4_t<Type> &n) const
    {
        mat4_t<Type> res;
        Type *row, *column;

        for (int i = 0; i < 16; ++i)
        {
            row = &data[i % 4];
            column = &n.data[4 * (i / 4)];
            res[i] = 0;
            for (int j = 0; j < 4; ++j)
            {
                res[i] += row[4 * j] * column[j];
            }
        }

        return res;
    }

    inline mat4_t<Type> &
    operator=(const mat4_t<Type> &n)
    {
        memcpy(data, n.data, 16 * sizeof(Type));
        return *this;
    }

    inline mat4_t<Type> &
    operator=(mat4_t<Type> &&n)
    {
        this->data = n.data;
        n.data = NULL;
        return *this;
    }

    inline bool
    operator==(const mat4_t<Type> &mat) const
    {
        for (unsigned int i = 0; i < 16; ++i) {
            if (this->data[i] != mat.data[i])
                return false;
        }

        return true;
    }

    inline Type &
    operator[](unsigned int index)
    {
        return data[index];
    }

    // TODO name this better
    vec3 getTranslation() const
    {
        return vec3(data[12], data[13], data[14]);
    }

    inline quat_t<Type>
    getQuat() const
    {
        const Type w = sqrt(1.f + data[0] +
                             data[5] + data[10]) / 2.f;
        const Type div = 1.f / (4.f * w);
        const Type x = (data[6] - data[9]) * div;
        const Type y = (data[8] - data[2]) * div;
        const Type z = (data[1] - data[4]) * div;

        quat_t<Type> lquat(w, x, y, z);
        lquat.normalize();
        return lquat;
    }

    inline std::string
    format() const
    {
        char output[128];
        snprintf(output, 512, "\n"
                 "[ %.2f %.2f %.2f %.2f ]\n"
                 "[ %.2f %.2f %.2f %.2f ]\n"
                 "[ %.2f %.2f %.2f %.2f ]\n"
                 "[ %.2f %.2f %.2f %.2f ]\n",
                 data[0], data[4], data[8], data[12],
                 data[1], data[5], data[9], data[13],
                 data[2], data[6], data[10], data[14],
                 data[3], data[7], data[11], data[15]);

        return std::string(output);
    }

    static inline mat4_t<Type>
    identity()
    {
        return mat4_t<Type>();
    }

    static inline mat4_t<Type>
    rotation(Type angle, Type x, Type y, Type z)
    {
        const Type s = sin(angle);
        const Type c = cos(angle);

        return mat4_t<Type>(
            { x * x * (1 - c) + c, y * x * (1 - c) + z * s,
              x * z * (1 - c) - y * s, 0.f,
              x * y * (1 - c) - z * s, y * y * (1 - c) + c,
              y * z * (1 - c) + x * s, 0.f,
              x * z * (1 - c) + y * s, y * z * (1 - c) - x * s,
              z * z * (1 - c) + c, 0.f,
              0, 0, 0, 1});
    }

    static inline mat4_t<Type>
    translation(const vec_t<Type, 3> &trans)
    {
        const Type x = trans.x();
        const Type y = trans.y();
        const Type z = trans.z();
        return mat4_t<Type>(
            { 1.f, 0.f, 0.f, 0.f,
              0.f, 1.f, 0.f, 0.f,
              0.f, 0.f, 1.f, 0.f,
              x, y, z, 1.f });
    }

    static inline mat4_t<Type>
    scale(Type x, Type y, Type z)
    {
        return mat4_t<Type>(
            { x, 0.f, 0.f, 0.f,
              0.f, y, 0.f, 0.f,
              0.f, 0.f, z, 0.f,
              0.f, 0.f, 0.f, 1.f });
    }

    static inline mat4_t<Type>
    perspective(Type, Type,
                Type, Type,
                Type, Type)
    {
        // TODO
        assert(false);
        return mat4_t<Type>();
    }

    static inline mat4_t<Type>
    ortho(Type, Type,
          Type, Type,
          Type, Type)
    {
        // TODO
        assert(false);
        return mat4_t<Type>();
    }

    Type *data = new Type[16];
};

typedef mat4_t<float> mat4;
typedef mat4_t<double> mat4d;

#endif // __MATRIX_H__
