#ifndef MPOLYGON_H
#define MPOLYGON_H
#include <QPainter>
#include "MMatrix.h"

class MPolygon
{
private:
    MVector4D A,B,C;
public:
    MPolygon();
    MPolygon(MVector4D A, MVector4D B, MVector4D C): A(A), B(B), C(C) {}

    void drawPoly(QPainter & p);
};

#endif // MPOLYGON_H
