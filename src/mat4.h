#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <cstring>
#include <string>
#include <initializer_list>

#include "vec.h"

template <typename Type>
class mat4_t
{
public:

    mat4_t()
        : data(new Type[16])
    {
        data = {1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
    }

    mat4_t(const Type *n)
        : data(new Type[16])
    {
        memcpy(data, n, 16 * sizeof(Type));
    }

    mat4_t(std::initializer_list<Type> values)
        : data(new Type[16])
    {
        int i = 0;
        for (const Type value: values)
        {
            if (i >= 16) break;

            data[i++] = value;
        }
        for (; i < 16; ++i)
        {
            i == 0 ||
                i == 5 ||
                i == 10 ||
                i == 15 ? data[i] = 1 : data[i] = 0;
        }
    }

    mat4_t(const mat4_t &n)
        : data(new Type[16])
    {
        operator=(n);
    }

    mat4_t(mat4_t &&n)
    {
        this->data = n.data;
        n.data = NULL;
    }

	mat4_t(const vec4 &base_x,
           const vec4 &base_y,
           const vec4 &base_z)
        : data(new Type[16])
    {
        this->data = {base_x.x(), base_x.y(), base_x.z(), 0.f,
                      base_y.x(), base_y.y(), base_y.z(), 0.f,
                      base_z.x(), base_z.y(), base_z.z(), 0.f,
                      0.f, 0.f, 0.f, 1.f};
    }

    mat4_t(const vec_t<Type, 3> &axis, Type angle)
        : data(new Type[16])
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/
        const Type c = cos(angle);
        const Type s = sin(angle);
        const Type t = 1 - c;
        const vec_t<Type, 3> na = axis / length(axis);
        const Type x = na.x();
        const Type y = na.y();
        const Type z = na.z();

        this->data = {t * x * x + c, t * x * y - z * s, t * x * z + y * s, 0,
                      t * x * y + z * s, t * y * y + c, t * y * z - x * s, 0,
                      t * x * z - y * s, t * y * z + x * s, t * z * z + c, 0,
                      0, 0, 0, 1};
    }

    ~mat4_t()
    {
        if (data)
        {
            delete [] data;
            data = NULL;
        }
    }

    mat4_t<Type>
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

    mat4_t<Type>
    transpose() const
    {
        return mat4_t<Type>({ data[0], data[4], data[8], data[12],
                    data[1], data[5], data[9], data[13],
                    data[2], data[6], data[10], data[14],
                    data[3], data[7], data[11], data[15] });
    }

    mat4_t<Type>
    operator+(const mat4_t<Type> &mat) const
    {
        Type data[16];

        for (int i = 0; i < 16; ++i)
        {
            data[i] = this->data[i] + mat.data[i];
        }

        return mat4_t<Type>(data);
    }

    mat4_t<Type>
    operator-(const mat4_t<Type> &mat) const
    {
        Type data[16];

        for (int i = 0; i < 16; ++i)
        {
            data[i] = this->data[i] - mat.data[i];
        }

        return mat4_t<Type>(data);
    }

    vec_t<Type, 4>
    operator*(const vec_t<Type, 4> &vec) const
    {
        return vec4(data[0] * vec.x() + data[4] * vec.y() + data[8] * vec.z() +
                    data[12] * vec.w(),
                    data[1] * vec.x() + data[5] * vec.y() + data[9] * vec.z() +
                    data[13] * vec.w(),
                    data[2] * vec.x() + data[6] * vec.y() + data[10] * vec.z() +
                    data[14] * vec.w(),
                    data[3] * vec.x() + data[7] * vec.y() + data[11] * vec.z() +
                    data[15] * vec.w());
    }

    mat4_t<Type>
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

    mat4_t<Type> &
    operator=(const mat4_t<Type> &n)
    {
        memcpy(data, n.data, 16 * sizeof(Type));
        return *this;
    }

    mat4_t<Type> &
    operator=(mat4_t<Type> &&n)
    {
        this->data = n.data;
        n.data = NULL;
        return *this;
    }

    bool
    operator==(const mat4_t<Type> &mat) const
    {
        for (unsigned int i = 0; i < 16; ++i) {
            if (this->data[i] != mat.data[i])
                return false;
        }

        return true;
    }

    Type &
    operator[](unsigned int index)
    {
        return data[index];
    }

    // TODO name this better
    vec3 getTranslation() const
    {
        return vec3(data[12], data[13], data[14]);
    }

    std::string
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

    static mat4_t<Type>
    identity()
    {
        return mat4_t<Type>();
    }

    static mat4_t<Type>
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

    static mat4_t<Type>
    translation(Type x, Type y, Type z)
    {
        return mat4_t<Type>(
            { 1.f, 0.f, 0.f, 0.f,
              0.f, 1.f, 0.f, 0.f,
              0.f, 0.f, 1.f, 0.f,
              x, y, z, 1.f });
    }

    static mat4_t<Type>
    scale(Type x, Type y, Type z)
    {
        return mat4_t<Type>(
            { x, 0.f, 0.f, 0.f,
              0.f, y, 0.f, 0.f,
              0.f, 0.f, z, 0.f,
              0.f, 0.f, 0.f, 1.f });
    }

    static mat4_t<Type>
    perspective(Type left, Type right,
                Type top, Type bottom,
                Type near, Type far)
    {
        // TODO
        assert(false);
        return mat4_t<Type>();
    }

    static mat4_t<Type>
    ortho(Type left, Type right,
          Type top, Type bottom,
          Type near, Type far)
    {
        // TODO
        assert(false);
        return mat4_t<Type>();
    }

    Type *data;
};

typedef mat4_t<float> mat4;
typedef mat4_t<double> mat4d;

#endif // __MATRIX_H__
