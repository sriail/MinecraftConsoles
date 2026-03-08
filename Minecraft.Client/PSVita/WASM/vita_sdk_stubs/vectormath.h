#pragma once

#include <cmath>

namespace sce {
namespace Vectormath {
namespace Simd {
namespace Aos {

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
};

struct Vector4 {
    float x, y, z, w;
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
    Vector4 operator+(const Vector4 &o) const { return Vector4(x + o.x, y + o.y, z + o.z, w + o.w); }
    Vector4 operator-(const Vector4 &o) const { return Vector4(x - o.x, y - o.y, z - o.z, w - o.w); }
    Vector4 operator*(float s) const { return Vector4(x * s, y * s, z * s, w * s); }
};

struct Quat {
    float x, y, z, w;
    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
};

struct Matrix4 {
    Vector4 col[4];
    Matrix4() {}
    static inline Matrix4 identity() {
        Matrix4 m;
        m.col[0] = Vector4(1, 0, 0, 0);
        m.col[1] = Vector4(0, 1, 0, 0);
        m.col[2] = Vector4(0, 0, 1, 0);
        m.col[3] = Vector4(0, 0, 0, 1);
        return m;
    }
};

inline Vector4 normalize(Vector4 v) {
    float len2 = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    if (len2 > 0.0f) {
        float invLen = 1.0f / sqrtf(len2);
        return Vector4(v.x * invLen, v.y * invLen, v.z * invLen, v.w * invLen);
    }
    return v;
}
inline float dot(Vector4 a, Vector4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
inline Vector4 cross(Vector4 a, Vector4 b) {
    return Vector4(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        0.0f
    );
}

} // namespace Aos
} // namespace Simd
} // namespace Vectormath
} // namespace sce
