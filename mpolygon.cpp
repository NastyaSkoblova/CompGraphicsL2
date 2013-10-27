#include "mpolygon.h"
#include "MMatrix.h"

MPolygon::MPolygon()
{
    A = MVector4D(0,0,0,1);
    B = MVector4D(0,0,0,1);
    C = MVector4D(0,0,0,1);
}


void MPolygon::drawPoly(QPainter &p)
{
    p.drawLine(A.x(),A.y(), B.x(),B.y());
    p.drawLine(B.x(),B.y(), C.x(),C.y());
    p.drawLine(C.x(),C.y(), A.x(),A.y());
}


void MPolyObject::pushPoly(MPolygon P){
    List->push_back(P);
}

void MPolyObject::drawPolyObject(QPainter &p){
    for(auto mp : *List){
        mp.drawPoly(p);
    }
}

void MPolyObject::drowShadowObj(QPainter &p, MVector4D & source)
{
    MMatrix4D M(-source.z(),0,source.x(),0,
                 0,-source.z(),0,0,
                 0,0,source.y(),0,
                 0,0,0,-source.z());
    M.transport(0,200,0);
    for(auto mp : *List){
        MPolygon sp(mp);
        sp.A = M*sp.A;
        sp.A.homogenization();
        sp.B = M*sp.B;
        sp.B.homogenization();
        sp.C = M*sp.C;
        sp.C.homogenization();
        sp.drawPoly(p);
    }
}

void MPolyObject::hideInvisible() {
    for(std::list<MPolygon>::iterator i=List->begin(), j; i != List->end() ;){
        if(  (((*i).B-(*i).A)^((*i).B-(*i).C)).z() <= 0  ){
            j = i;
            i++;
            List->erase(j);
        } else
            i++;
    }
}

void MPolyObject::drawColoredObj(QPainter &p, QColor color)
{
    for(auto mp : *List) {
        MColoredPolygon(mp,color).drawPoly(p);
    }
}

void MPolyObject::drawColoredObjWithLight(QPainter &p, QColor color, MVector4D & source)
{
    MVector4D CP, Norm;
    for(auto mp : *List) {
        Norm = ((mp.B-mp.A)^(mp.B-mp.C));
        if (Norm.z() > 0) {
            CP = calcLight(mp.A, Norm,source);
            MColoredPolygon(mp,QColor(CP.x()*color.red(),
                                      CP.y()*color.green(),
                                      CP.z()*color.blue())).drawPoly(p);
        }
    }
}


MColoredPolygon::MColoredPolygon()
{
    A = MVector4D(0,0,0,1);
    B = MVector4D(0,0,0,1);
    C = MVector4D(0,0,0,1);
    color = QColor(255,255,255);
}

void MColoredPolygon::drawPoly(QPainter &p)
{
    QPainterPath path;
    path.moveTo(A.x(),A.y());
    path.lineTo(B.x(),B.y());
    path.lineTo(C.x(),C.y());
    path.lineTo(A.x(),A.y());
    p.fillPath(path,color);

}


MVector4D calcLight(MVector4D P, MVector4D &N, MVector4D L){
    P.normalize();
    N.normalize();
    double intens = ((P-N).normalize())*((P-L).normalize());
    //std::cout << P-N << std::endl;
    if (intens < 0) intens = 0;
    return MVector4D(intens ,intens,intens, 1);
}
