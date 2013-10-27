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

void MPolyObject::hideInvisible() {
    for(std::list<MPolygon>::iterator i=List->begin(), j; i != List->end() ;){
        MVector4D tmp(((*i).B-(*i).A)^((*i).B-(*i).C));
        if(tmp.z() > 0){
            j = i;
            i++;
            List->erase(j);
        } else
            i++;
    }
}
