#ifndef MPOLYGON_H
#define MPOLYGON_H
#include <QPainter>
#include "MMatrix.h"
#include <list>

class MPolygon
{
private:
    MVector4D A,B,C;
public:
    MPolygon();
    MPolygon(MVector4D A, MVector4D B, MVector4D C): A(A), B(B), C(C) {}

    void drawPoly(QPainter & p);
    friend class MPolyObject;
};

class MPolyObject
{
private:
    std::list<MPolygon> *List;
public:
    MPolyObject(): List(new std::list<MPolygon>){}
    void pushPoly(MPolygon P);
    void drawPolyObject(QPainter & p);
    void hideInvisible();


};

#endif // MPOLYGON_H
