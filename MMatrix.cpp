#include "MMatrix.h"
float MVector3D::x() const
{
    return cx;
}

float MVector3D::y() const
{
    return cy;
}

float MVector3D::z() const
{
    return cz;
}

MVector4D::MVector4D(){
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
}

MVector4D::MVector4D(float x, float y, float z, float w)
{
    p[0] = x;
    p[1] = y;
    p[2] = z;
    p[3] = w;
}

MVector4D::MVector4D(float x, float y, float z)
{
    p[0] = x;
    p[1] = y;
    p[2] = z;
    p[3] = 0;
}

float MVector4D::x() const
{
    return p[0];
}

float MVector4D::y() const
{
    return p[1];
}

float MVector4D::z() const
{
    return p[2];
}

float MVector4D::w() const
{
    return p[3];
}

void MVector4D::setX(float xx)
{
    p[0] = xx;
}

void MVector4D::setY(float yy)
{
    p[1] = yy;
}

void MVector4D::setZ(float zz)
{
    p[2] = zz;
}

void MVector4D::setW(float ww)
{
    p[3] = ww;
}

void MVector4D::printVector(std::ostream &out) const
{
    out << p[0] << ' ' << p[1] << ' ' << p[2] << ' ' << p[3] << '\n';
}

MVector4D MVector4D::operator-(const MVector4D &other) const {
    return MVector4D(p[0]-other.p[0], p[1]-other.p[1], p[2]-other.p[2]);
}

MVector4D MVector4D::operator^(const MVector4D &other) const
{
    return MVector4D((p[1]*other.p[2]-p[2]*other.p[1]),
                     -(p[0]*other.p[2]-p[2]*other.p[0]),
            (p[0]*other.p[1]-p[1]*other.p[0]));
}

MVector4D MVector4D::operator+(const MVector4D &other) const
{
    return MVector4D(p[0]+other.p[0], p[1]+other.p[1], p[2]+other.p[2]);
}

float MVector4D::operator*(const MVector4D &other) const
{
    return p[0]*other.p[0]+p[1]*other.p[1]+p[2]*other.p[2];
}

void MVector4D::homogenization(){
    p[0] = p[0]/p[3];
    p[1] = p[1]/p[3];
    p[2] = p[2]/p[3];
    p[3] = 1;
}

float MVector4D::abs()
{
    return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}

MVector4D MVector4D::normalize()
{
    float l = abs();
    p[0] = p[0]/l;
    p[1] = p[1]/l;
    p[2] = p[2]/l;
    return *this;
}


MMatrix3D::MMatrix3D(float a11, float a12, float a13,
                     float a21, float a22, float a23,
                     float a31, float a32, float a33)
{
    m[0][0] = a11; m[0][1] = a12; m[0][2] = a13;
    m[1][0] = a21; m[1][1] = a22; m[1][2] = a23;
    m[2][0] = a31; m[2][1] = a32; m[2][2] = a33;
}


MMatrix3D::MMatrix3D()
{
    m[0][0] = 0; m[0][1] = 0; m[0][2] = 0;
    m[1][0] = 0; m[1][1] = 0; m[1][2] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 0;
}

MMatrix3D MMatrix3D::operator*(const MMatrix3D &other) const
{
    MMatrix3D res;
    for(int i=0; i < 3; i++){
        for (int j=0; j < 3; j++){
            for(int k=0; k < 3; k++){
                res.m[i][j] += m[i][k]*other.m[k][j];
            }
        }
    }
    return res;
}

void MMatrix3D::printMatrix(std::ostream &out) const
{
    out << m[0][0] << ' ' << m[0][1] << ' ' << m[0][2] << std::endl;
    out << m[1][0] << ' ' << m[1][1] << ' ' << m[1][2] << std::endl;
    out << m[2][0] << ' ' << m[2][1] << ' ' << m[2][2] << std::endl;
}


MMatrix4D::MMatrix4D()
{
    m[0][0] = 0; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
    m[1][0] = 0; m[1][1] = 0; m[1][2] = 0; m[0][3] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 0; m[0][3] = 0;
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 0;
}

MMatrix4D::MMatrix4D(float a11, float a12, float a13, float a14,
                     float a21, float a22, float a23, float a24,
                     float a31, float a32, float a33, float a34,
                     float a41, float a42, float a43, float a44)
{
    m[0][0] = a11; m[0][1] = a12; m[0][2] = a13; m[0][3] = a14;
    m[1][0] = a21; m[1][1] = a22; m[1][2] = a23; m[1][3] = a24;
    m[2][0] = a31; m[2][1] = a32; m[2][2] = a33; m[2][3] = a34;
    m[3][0] = a41; m[3][1] = a42; m[3][2] = a43; m[3][3] = a44;
}

MMatrix4D MMatrix4D::operator*(const MMatrix4D &other) const
{
    MMatrix4D res;
    for(int i=0; i < 4; i++){
        for (int j=0; j < 4; j++){
            for(int k=0; k < 4; k++){
                res.m[i][j] += m[i][k]*other.m[k][j];
            }
        }
    }
    return res;
}

MVector4D MMatrix4D::operator*(const MVector4D &v) const
{
    return MVector4D(m[0][0]*v.x()+m[0][1]*v.y()+m[0][2]*v.z()+m[0][3]*v.w(),
                     m[1][0]*v.x()+m[1][1]*v.y()+m[1][2]*v.z()+m[1][3]*v.w(),
                     m[2][0]*v.x()+m[2][1]*v.y()+m[2][2]*v.z()+m[2][3]*v.w(),
                     m[3][0]*v.x()+m[3][1]*v.y()+m[3][2]*v.z()+m[3][3]*v.w());
}

MMatrix4D MMatrix4D::rotateX(float angle)
{
    float ca = cos(angle);
    float sa = sin(angle);
    MMatrix4D r(1, 0,  0,0,
                0,ca,-sa,0,
                0,sa, ca,0,
                0, 0,  0,1);
    *this = r*(*this);
    return *this;

}

MMatrix4D MMatrix4D::rotateY(float angle)
{
    float ca = cos(angle);
    float sa = sin(angle);
    MMatrix4D r(ca, 0,-sa,0,
                 0, 1,  0,0,
               sa,  0, ca,0,
                 0, 0,  0,1);
    *this = r*(*this);
    return *this;
}

MMatrix4D MMatrix4D::rotateZ(float angle)
{
    float ca = cos(angle);
    float sa = sin(angle);
    MMatrix4D r(ca,-sa, 0,0,
                sa, ca, 0,0,
                 0,  0, 1,0,
                 0,  0, 0,1);
    *this = r*(*this);
    return *this;
}

MMatrix4D MMatrix4D::transport(float X, float Y, float Z)
{
    MMatrix4D r(1, 0, 0, X,
                0, 1, 0, Y,
                0, 0, 1, Z,
                0, 0, 0, 1);
    *this = r*(*this);
    return *this;
}

MMatrix4D MMatrix4D::scale(float X, float Y, float Z)
{
    MMatrix4D r(X, 0, 0, 0,
                0, Y, 0, 0,
                0, 0, Z, 0,
                0, 0, 0, 1);
    *this = r*(*this);
    return *this;
}

void MMatrix4D::printMatrix(std::ostream &out) const
{
    out << m[0][0] << ' ' << m[0][1] << ' ' << m[0][2] << ' ' << m[0][3] << std::endl;
    out << m[1][0] << ' ' << m[1][1] << ' ' << m[1][2] << ' ' << m[1][3] << std::endl;
    out << m[2][0] << ' ' << m[2][1] << ' ' << m[2][2] << ' ' << m[2][3] << std::endl;
    out << m[3][0] << ' ' << m[3][1] << ' ' << m[3][2] << ' ' << m[3][3] << std::endl;
}

std::ostream &operator <<(std::ostream &out, const MMatrix3D &obj){
    obj.printMatrix(out);
    return out;

}

std::ostream &operator <<(std::ostream &out, const MMatrix4D &obj){
    obj.printMatrix(out);
    return out;

}

std::ostream &operator <<(std::ostream &out, const MVector4D &obj)
{
    obj.printVector(out);
    return out;
}


