#include "MMatrix.h"
double MVector3D::x() const
{
    return cx;
}

double MVector3D::y() const
{
    return cy;
}

double MVector3D::z() const
{
    return cz;
}

double MVector4D::x() const
{
    return cx;
}

double MVector4D::y() const
{
    return cy;
}

double MVector4D::z() const
{
    return cz;
}

double MVector4D::w() const
{
    return cw;
}

void MVector4D::setX(double xx)
{
    cx = xx;
}

void MVector4D::setY(double yy)
{
    cy = yy;
}

void MVector4D::setZ(double zz)
{
    cz = zz;
}

void MVector4D::setW(double ww)
{
    cw = ww;
}

void MVector4D::printVector(std::ostream &out) const
{
    out << cx << ' ' << cy << ' ' << cz << ' ' << cw << '\n';
}

MVector4D MVector4D::operator-(const MVector4D &other) const {
    return MVector4D(cx-other.cx, cy-other.cy, cz-other.cz);
}

MVector4D MVector4D::operator^(const MVector4D &other) const
{
    return MVector4D((cy*other.cz-cz*other.cy),
                     -(cx*other.cz-cz*other.cx),
                     (cx*other.cy-cy*other.cx));
}

double MVector4D::operator*(const MVector4D &other) const
{
    return cx*other.cx+cy*other.cy+cz*other.cz;
}

void MVector4D::homogenization(){
    cx = cx/cw;
    cy = cy/cw;
    cz = cz/cw;
    cw = 1;
}

double MVector4D::abs()
{
    return sqrt(cx*cx+cy*cy+cz*cz);
}

MVector4D MVector4D::normalize()
{
    double l = abs();
    cx = cx/l;
    cy = cy/l;
    cz = cz/l;
    return *this;
}


MMatrix3D::MMatrix3D(double a11, double a12, double a13,
                     double a21, double a22, double a23,
                     double a31, double a32, double a33)
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

MMatrix4D::MMatrix4D(double a11, double a12, double a13, double a14,
                     double a21, double a22, double a23, double a24,
                     double a31, double a32, double a33, double a34,
                     double a41, double a42, double a43, double a44)
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

MMatrix4D MMatrix4D::rotateX(double angle)
{
    double ca = cos(angle);
    double sa = sin(angle);
    MMatrix4D r(1, 0,  0,0,
                0,ca,-sa,0,
                0,sa, ca,0,
                0, 0,  0,1);
    *this = r*(*this);
    return *this;

}

MMatrix4D MMatrix4D::rotateY(double angle)
{
    double ca = cos(angle);
    double sa = sin(angle);
    MMatrix4D r(ca, 0,-sa,0,
                 0, 1,  0,0,
               sa,  0, ca,0,
                 0, 0,  0,1);
    *this = r*(*this);
    return *this;
}

MMatrix4D MMatrix4D::rotateZ(double angle)
{
    double ca = cos(angle);
    double sa = sin(angle);
    MMatrix4D r(ca,-sa, 0,0,
                sa, ca, 0,0,
                 0,  0, 1,0,
                 0,  0, 0,1);
    *this = r*(*this);
    return *this;
}

MMatrix4D MMatrix4D::transport(double X, double Y, double Z)
{
    MMatrix4D r(1, 0, 0, X,
                0, 1, 0, Y,
                0, 0, 1, Z,
                0, 0, 0, 1);
    *this = r*(*this);
    return *this;
}

MMatrix4D MMatrix4D::scale(double X, double Y, double Z)
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
