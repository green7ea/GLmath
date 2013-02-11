#include "mat4f.h"

#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include "quat.h"

static void
stupidFunctionBecauseC11IsntImplementedYet(Mat4f * mat,
                                           const Quat &quat,
                                           const vec4 &vec)
{
    mat->data[0] = 1.f - 2.f * (quat.y * quat.y - quat.z * quat.z);
    mat->data[1] = 2.f * (quat.x * quat.y + quat.z * quat.w);
    mat->data[2] = 2.f * (quat.x * quat.z - quat.y * quat.w);
    mat->data[3] = 0.f;
    mat->data[4] = 2.f * (quat.x * quat.y - quat.z * quat.w);
    mat->data[5] = 1.f - 2.f * (quat.x * quat.x - quat.z * quat.z);
    mat->data[6] = 2.f * (quat.y * quat.z + quat.x * quat.w);
    mat->data[7] = 0.f;
    mat->data[8] = 2.f * (quat.x * quat.z + quat.y * quat.w);
    mat->data[9] = 2.f * (quat.y * quat.z - quat.x * quat.w);
    mat->data[10] = 1.f - 2.f * (quat.x * quat.x - quat.y * quat.y);
    mat->data[11] = 0.f;
    mat->data[12] = vec.x();
    mat->data[13] = vec.y();
    mat->data[14] = vec.z();
    mat->data[15] = 1.f;
}

Mat4f::Mat4f():
    data(new float[16])
{
    this->data[0] = this->data[5] = this->data[10] = this->data[15] = 1.f;
    this->data[1] = this->data[2] = this->data[3] = 0.f;
    this->data[4] = this->data[6] = this->data[7] = 0.f;
    this->data[8] = this->data[9] = this->data[11] = 0.f;
    this->data[12] = this->data[13] = this->data[14] = 0.f;
}

Mat4f::Mat4f(const float *n):
    data(new float[16])
{
    memcpy(data, n, 16 * sizeof(float));
}

Mat4f::Mat4f(const Mat4f &n):
    data(new float[16])
{
    operator=(n);
}

Mat4f::Mat4f(const Quat &quat):
    data(new float[16])
{
    stupidFunctionBecauseC11IsntImplementedYet(this, quat, vec4());
}

Mat4f::Mat4f(const vec4 &vec):
    data(new float[16])
{
    stupidFunctionBecauseC11IsntImplementedYet(this, Quat(), vec);
}

Mat4f::Mat4f(const Quat &quat, const vec4 &vec):
    data(new float[16])
{
    stupidFunctionBecauseC11IsntImplementedYet(this, quat, vec);
}

Mat4f::Mat4f(const vec4 &base_x,
             const vec4 &base_y,
             const vec4 &base_z):
    data(new float[16])
{
    this->data[0] = base_x.x();
    this->data[4] = base_x.y();
    this->data[8] = base_x.z();
    this->data[12] = 0.f;

    this->data[1] = base_y.x();
    this->data[5] = base_y.y();
    this->data[9] = base_y.z();
    this->data[13] = 0.f;

    this->data[2] = base_z.x();
    this->data[6] = base_z.y();
    this->data[10] = base_z.z();
    this->data[14] = 0.f;

    this->data[3] = 0.f;
    this->data[7] = 0.f;
    this->data[11] = 0.f;
    this->data[15] = 1.f;
}

Mat4f::~Mat4f()
{
    if (data)
    {
        delete [] data;
    }
}

Mat4f
Mat4f::inverse() const
{
    const float *mat = this->data;
    float dst[16];
    float tmp[12];
    float src[16];
    float det;

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

    return Mat4f(dst);
}

std::string
Mat4f::format() const
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

Mat4f
Mat4f::transpose() const
{
    const float data[] =
        { this->data[0], this->data[4], this->data[8], this->data[12],
          this->data[1], this->data[5], this->data[9], this->data[13],
          this->data[2], this->data[6], this->data[10], this->data[14],
          this->data[3], this->data[7], this->data[11], this->data[15] };
    return Mat4f(data);
}

Mat4f
Mat4f::operator+(const Mat4f &mat) const
{
    float data[16];

    for (int i = 0; i < 16; ++i)
    {
        data[i] = this->data[i] + mat.data[i];
    }

    return Mat4f(data);
}

Mat4f
Mat4f::operator-(const Mat4f &mat) const
{
    float data[16];

    for (int i = 0; i < 16; ++i)
    {
        data[i] = this->data[i] - mat.data[i];
    }

    return Mat4f(data);
}

Mat4f
Mat4f::operator*(const Mat4f &n) const
{
    Mat4f res;
    float *row, *column;

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

vec4
Mat4f::operator*(const vec4 &vec) const
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

Mat4f &
Mat4f::operator=(const Mat4f &n)
{
    memcpy(data, n.data, 16 * sizeof(float));
    return *this;
}

Mat4f &
Mat4f::operator=(Mat4f &&n)
{
    this->data = n.data;
    n.data = NULL;
    return *this;
}

bool
Mat4f::operator==(const Mat4f &mat) const
{
    for (unsigned int i = 0; i < 16; ++i) {
        if (this->data[i] != mat.data[i])
            return false;
    }

    return true;
}

float &
Mat4f::operator[] (unsigned int i)
{
    return data[i];
}

vec4
Mat4f::getTranslation() const
{
    return vec4(data[12], data[13], data[14]);
}

Mat4f
Mat4f::identity()
{
    return Mat4f();
}

Mat4f
Mat4f::rotation(float angle, float x, float y, float z)
{
    const float     s = sin(angle);
    const float     c = cos(angle);
    const float     data[] =
        { x * x * (1 - c) + c, y * x * (1 - c) + z * s,
          x * z * (1 - c) - y * s, 0.f,
          x * y * (1 - c) - z * s, y * y * (1 - c) + c,
          y * z * (1 - c) + x * s, 0.f,
          x * z * (1 - c) + y * s, y * z * (1 - c) - x * s,
          z * z * (1 - c) + c, 0.f,
          0.f, 0.f, 0.f, 1.f
        };

    return Mat4f(data);
}

Mat4f
Mat4f::translation(float x, float y, float z)
{
    const float data[] = { 1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           x, y, z, 1.f };
    return Mat4f(data);
}

Mat4f
Mat4f::scale(float x, float y, float z)
{
    const float data[] = { x, 0.f, 0.f, 0.f,
                           0.f, y, 0.f, 0.f,
                           0.f, 0.f, z, 0.f,
                           0.f, 0.f, 0.f, 1.f };
    return Mat4f(data);
}

Mat4f
Mat4f::perspective(float left, float right,
                   float top, float bottom, float near, float far)
{
    // TODO
    assert(false);
    return Mat4f();
}

Mat4f
Mat4f::ortho(float left, float right,
             float top, float bottom, float near, float far)
{
    // TODO
    assert(false);
    return Mat4f();
}
