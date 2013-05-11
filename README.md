vector-math
===========

A simple C++ math library for computer graphics that tries to mimick
the basic types found in glsl as closely as possible. It provides
matrices of variable sizes, vectors of variables size and quaternions.

To build from the project directory:

autoconf

./configure

make install


The library is currently in alpha state and should be used at your own
risk. Most features have had a little testing and should produce
correct results. Planned features:

- Aggressive use of C++11 move to limit temporaries.
- Code modifications to enable auto vectorization from smart compilers.
- Fixed point type to speed up on architectures with slow float calculations.
- Benchmarking against similar libraries (eigen, bullet, SIMDx86).
