#ifndef MMATRIX_H
#define MMATRIX_H

#include <iostream>
#include <cmath>

class MVector3D
{
private:
    double cx;
    double cy;
    double cz;

public:
    MVector3D(): cx(0), cy(0), cz(0){}
    MVector3D(double x, double y, double z): cx(x), cy(y), cz(z){}

    double x() const;
    double y() const;
    double z() const;
};

class MVector4D
{
private:
    double cx;
    double cy;
    double cz;
    double cw;

public:
    MVector4D(): cx(0), cy(0), cz(0), cw(0) {}
    MVector4D(double x, double y, double z, double w): cx(x), cy(y), cz(z), cw(w){}
    MVector4D(double x, double y, double z): cx(x), cy(y), cz(z), cw(0){}

    double x() const;
    double y() const;
    double z() const;
    double w() const;
    void setX(double xx);
    void setY(double yy);
    void setZ(double zz);
    void setW(double ww);
    void printVector(std::ostream & out) const;
    MVector4D operator-(const MVector4D & other) const;
    MVector4D operator^(const MVector4D & other) const;
    double operator*(const MVector4D & other) const;
    void homogenization();
    MVector4D normalize();
    double abs();
};

class MMatrix3D
{
private:
    double m[3][3];

public:

    MMatrix3D(double a11, double a12, double a13,
              double a21, double a22, double a23,
              double a31, double a32, double a33);
    MMatrix3D();

    MMatrix3D operator*(const MMatrix3D & other) const;

    void printMatrix(std::ostream & out) const;

};

class MMatrix4D
{
private:
    double m[4][4];

public:

    MMatrix4D();
    MMatrix4D(double a11, double a12, double a13, double a14,
              double a21, double a22, double a23, double a24,
              double a31, double a32, double a33, double a34,
              double a41, double a42, double a43, double a44);

    MMatrix4D operator*(const MMatrix4D & other) const;
    MVector4D operator*(const MVector4D & vect) const;

    MMatrix4D rotateX(double angle);
    MMatrix4D rotateY(double angle);
    MMatrix4D rotateZ(double angle);
    MMatrix4D transport(double X, double Y, double Z);
    MMatrix4D scale(double X, double Y, double Z);

    void printMatrix(std::ostream & out) const;
};

std::ostream &operator <<(std::ostream &out, const MMatrix3D & obj);
std::ostream &operator <<(std::ostream &out, const MMatrix4D & obj);
std::ostream &operator <<(std::ostream &out, const MVector4D & obj);


#endif // MMATRIX_H
