#ifndef MPOLYGON_H
#define MPOLYGON_H
#include <QPainter>
#include "MMatrix.h"
#include <list>

enum MLight{CookTorrance, Standart};

class MPolygon
{
protected:
    MVector4D A,B,C;
public:
    MPolygon();
    MPolygon(MVector4D A, MVector4D B, MVector4D C): A(A), B(B), C(C) {}

    void drawPoly(QPainter & p);
    MPolygon operator*(MMatrix4D & M) const;

    bool operator<(const MPolygon &Elem) const;
    friend class MPolyObject;
};

class MColoredPolygon : protected MPolygon
{
    QColor color;
public:
    MColoredPolygon();
    MColoredPolygon(MPolygon P, QColor color): MPolygon(P), color(color){}
    MColoredPolygon(MVector4D A, MVector4D B, MVector4D C, QColor color): MPolygon(A,B,C), color(color) {}

    void drawPoly(QPainter &p);
};
class MPolyObject
{
private:
    std::list<MPolygon> *Plist;
public:
    MPolyObject(): Plist(new std::list<MPolygon>){}
    void pushPoly(MPolygon P);
    void drawPolyObject(QPainter & p);
    void drawShadowObj(QPainter & p, MVector4D &source);
    void hideInvisible();
    void drawColoredObj(QPainter & p, QColor &color);
    void drawColoredObjWithLight(QPainter & p, QColor color, MVector4D & source, MLight model);


};

float calcLight(MVector4D P, MVector4D & N, MVector4D L);
float fresnel(float cosVN, float a);
float calcLightCookTorrance(MVector4D Norm, MVector4D source, MVector4D PoV, float fresnel, float roughness);
#endif // MPOLYGON_H
