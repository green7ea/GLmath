GLmath
======

A simple C++ math library for computer graphics that tries to mimick
the basic types found in GLSL as closely as possible. It provides
matrices of variable sizes, vectors of variables size and quaternions
of size 4.

Requirements
------------

- A C++11 compiler which supports nameless structs/unions (tested with
  g++ 4.9.2 and clang++ 3.5.1).

Building
--------

`autoconf`

`./configure`

`make install`


The library is currently in alpha state and should be used at your own
risk. Most features have had a little testing and should produce
correct results.