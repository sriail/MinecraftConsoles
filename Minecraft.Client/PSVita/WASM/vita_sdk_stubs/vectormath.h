#pragma once
// Minimal PS Vita vectormath.h stub for Emscripten/WASM builds.
// Provides the VectorMath types needed for PSVita math operations.
#include <math.h>

namespace Vectormath {
namespace Aos {

struct Vector2 {
    float x, y;
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vector4 {
    float x, y, z, w;
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

struct Matrix4 {
    Vector4 col[4];
    Matrix4() {}
};

struct Quat {
    float x, y, z, w;
    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

} // namespace Aos
} // namespace Vectormath
