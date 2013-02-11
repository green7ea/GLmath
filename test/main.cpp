#include <functional>
#include <cstdio>
#include <quat.h>
#include <vec.h>
#include <mat4.h>

bool eqv(float a, float b)
{
    return ((a - b) / (a + b)) < 0.00000001f;
}

void test(const char *name,
          const std::function<bool ()> &test_proc)
{
    try
    {
        const bool res = test_proc();
        if (res)
        {
            printf("\033[32m[OK]    ");
        }
        else
        {
            printf("\033[31m[FAIL]  ");
        }
    }
    catch (...)
    {
        printf("\033[31m[EXCPT] ");
    }

    printf("\033[0m%s\033[100C\n", name);
}

int main(int argc, char **argv)
{
    printf("\nVector tests:\n\n");
    test("Addition", []()
         {
             const vec4 a = {5.f, 9.f, 12034.f};
             const vec4 b = {1235.f, 0.f, 2346782.f};

             return a == a + b - b;
         });
    test("Multiplication", []()
         {
             const vec3 a = {5.f, 9.f, 12034.f};

             return vec3() == a * 0.f;
         });
    test("Cross Product", []()
         {
             const vec3 a(1.f, 0.f, 0.f);
             const vec3 b(0.f, 1.f, 0.f);
             const vec3 c(0.f, 0.f, 1.f);

             return c == cross(a, b);
         });
    test("Dot Product", []()
         {
             const vec4 a(43526.f, 877658.f, 425.f);
             const vec4 b(0.f, 1.f, 0.f);
             const vec4 c = cross(a, b);

             return 0.f == dot(a, c);
         });

    printf("\nMatrix tests:\n\n");

    test("Identity", []()
         {
             const vec4 a(10.f, 15.f, 20.f);
             const mat4 mat = mat4::identity();

             return a == mat * a;
         });

    printf("\nQuaternion tests:\n\n");
    test("Conservation of length", []()
         {
             const vec3 a(1235.f, 0.f, 2346782.f);
             const quat b(1.234f, vec3(1.f, 0.f, 1.f));

             return eqv(length(a), length(b * a));
         });

    return 0;
}
