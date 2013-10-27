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
