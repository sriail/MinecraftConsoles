#pragma once
/* vectormath.h — PS Vita vector math library stubs for Emscripten/WASM builds
 * The game may use a Sony VectorMath subset; provide the most common types. */

#include <math.h>

#ifdef __cplusplus

namespace sce {
namespace Vectormath {
namespace Simd {
namespace Aos {

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3 operator+(const Vector3 &o) const { return Vector3(x+o.x, y+o.y, z+o.z); }
    Vector3 operator-(const Vector3 &o) const { return Vector3(x-o.x, y-o.y, z-o.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
};

struct Vector4 {
    float x, y, z, w;
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
};

struct Matrix4 {
    float data[4][4];
    Matrix4() { for(int i=0;i<4;i++) for(int j=0;j<4;j++) data[i][j]=(i==j?1.f:0.f); }
};

} // Aos
} // Simd
} // Vectormath
} // sce

#endif /* __cplusplus */
