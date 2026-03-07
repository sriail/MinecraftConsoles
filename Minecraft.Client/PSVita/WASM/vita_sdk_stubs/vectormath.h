#pragma once
/*
 * Minimal PS Vita vectormath SDK stubs for Emscripten/WASM builds.
 * Provides sce::Vectormath::Simd::Aos::Vector4 and Matrix4 as used by
 * PSVitaMaths.h.
 */

namespace sce {
namespace Vectormath {
namespace Simd {
namespace Aos {

class Vector4 {
public:
    float x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    float getW() const { return w; }

    Vector4 operator+(const Vector4 &rhs) const { return Vector4(x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w); }
    Vector4 operator-(const Vector4 &rhs) const { return Vector4(x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w); }
    Vector4 operator*(float s)             const { return Vector4(x*s, y*s, z*s, w*s); }
};

class Matrix4 {
public:
    Vector4 col[4];

    Matrix4() {}
    Matrix4(const Vector4 &c0, const Vector4 &c1, const Vector4 &c2, const Vector4 &c3) {
        col[0] = c0; col[1] = c1; col[2] = c2; col[3] = c3;
    }
};

} /* namespace Aos */
} /* namespace Simd */
} /* namespace Vectormath */
} /* namespace sce */
