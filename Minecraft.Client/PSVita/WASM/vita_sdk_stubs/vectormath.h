#pragma once

namespace sce { namespace Vectormath { namespace Simd { namespace Aos {

class Vector4 {
public:
    float mX, mY, mZ, mW;

    Vector4() : mX(0), mY(0), mZ(0), mW(0) {}
    Vector4(float x, float y, float z, float w) : mX(x), mY(y), mZ(z), mW(w) {}

    float getX() const { return mX; }
    float getY() const { return mY; }
    float getZ() const { return mZ; }
    float getW() const { return mW; }

    void setX(float x) { mX = x; }
    void setY(float y) { mY = y; }
    void setZ(float z) { mZ = z; }
    void setW(float w) { mW = w; }

    Vector4 operator+(const Vector4& v) const { return Vector4(mX+v.mX, mY+v.mY, mZ+v.mZ, mW+v.mW); }
    Vector4 operator-(const Vector4& v) const { return Vector4(mX-v.mX, mY-v.mY, mZ-v.mZ, mW-v.mW); }
    Vector4 operator*(float s) const { return Vector4(mX*s, mY*s, mZ*s, mW*s); }
};

class Matrix4 {
public:
    Vector4 mCol0, mCol1, mCol2, mCol3;

    Matrix4() {}
    Matrix4(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3)
        : mCol0(c0), mCol1(c1), mCol2(c2), mCol3(c3) {}

    const Vector4& getCol(int i) const { return (&mCol0)[i]; }

    static Matrix4 identity() {
        return Matrix4(Vector4(1,0,0,0), Vector4(0,1,0,0), Vector4(0,0,1,0), Vector4(0,0,0,1));
    }
};

}}}}
