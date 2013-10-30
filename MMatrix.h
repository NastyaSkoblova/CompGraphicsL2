#ifndef MMATRIX_H
#define MMATRIX_H

#include <iostream>
#include <cmath>

class MVector3D
{
private:
    float cx;
    float cy;
    float cz;

public:
    MVector3D(): cx(0), cy(0), cz(0){}
    MVector3D(float x, float y, float z): cx(x), cy(y), cz(z){}

    float x() const;
    float y() const;
    float z() const;
};

class MVector4D
{
private:
    float p[4];

public:
    MVector4D();
    MVector4D(float x, float y, float z, float w);
    MVector4D(float x, float y, float z);

    float x() const;
    float y() const;
    float z() const;
    float w() const;
    void setX(float xx);
    void setY(float yy);
    void setZ(float zz);
    void setW(float ww);
    void printVector(std::ostream & out) const;
    MVector4D operator-(const MVector4D & other) const;
    MVector4D operator^(const MVector4D & other) const;
    MVector4D operator+(const MVector4D & other) const;
    float operator*(const MVector4D & other) const;
    void homogenization();
    MVector4D normalize();
    float abs();
};

class MMatrix3D
{
private:
    float m[3][3];

public:

    MMatrix3D(float a11, float a12, float a13,
              float a21, float a22, float a23,
              float a31, float a32, float a33);
    MMatrix3D();

    MMatrix3D operator*(const MMatrix3D & other) const;

    void printMatrix(std::ostream & out) const;

};

class MMatrix4D
{
private:
    float m[4][4];

public:

    MMatrix4D();
    MMatrix4D(float a11, float a12, float a13, float a14,
              float a21, float a22, float a23, float a24,
              float a31, float a32, float a33, float a34,
              float a41, float a42, float a43, float a44);

    MMatrix4D operator*(const MMatrix4D & other) const;
    MVector4D operator*(const MVector4D & vect) const;

    MMatrix4D rotateX(float angle);
    MMatrix4D rotateY(float angle);
    MMatrix4D rotateZ(float angle);
    MMatrix4D transport(float X, float Y, float Z);
    MMatrix4D scale(float X, float Y, float Z);

    void printMatrix(std::ostream & out) const;
};

std::ostream &operator <<(std::ostream &out, const MMatrix3D & obj);
std::ostream &operator <<(std::ostream &out, const MMatrix4D & obj);
std::ostream &operator <<(std::ostream &out, const MVector4D & obj);


#endif // MMATRIX_H
