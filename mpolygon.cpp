#include "mpolygon.h"
#include "MMatrix.h"
#define min(x,y) ((x<y) ? x: y)
#define max(x,y) ((x>y) ? x: y)


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

MPolygon MPolygon::operator*(MMatrix4D &M) const{
    return MPolygon(M*A, M*B, M*C);
}

bool MPolygon::operator<(const MPolygon &Elem) const
{
    return A.z() < Elem.A.z();
}

void MPolyObject::pushPoly(MPolygon P){
    Plist->push_back(P);
}

void MPolyObject::drawPolyObject(QPainter &p){
    for(auto mp : *Plist){
        mp.drawPoly(p);
    }
}

void MPolyObject::drawShadowObj(QPainter &p, MVector4D & source)
{
    MMatrix4D M(-source.z(),0,source.x(),0,
                 0,-source.z(),0,0,
                 0,0,source.y(),0,
                 0,0,0,-source.z());
    M.transport(0,200,0);
    for(auto mp : *Plist){
        MPolygon sp(mp);
        sp.A = M*sp.A;
        sp.A.homogenization();
        sp.B = M*sp.B;
        sp.B.homogenization();
        sp.C = M*sp.C;
        sp.C.homogenization();
        sp.drawPoly(p);
    }
    /*MMatrix4D M(1,0,0,0,
                0,0,0,0,
                0,0,1,0,
                0,0,0,1);
    //M.transport(0,200,0);
    for(auto mp: *Plist){
        (mp*M).drawPoly(p);
    }*/
}

void MPolyObject::hideInvisible() {
    for(std::list<MPolygon>::iterator i=Plist->begin(), j; i != Plist->end() ;){
        if(  (((*i).B-(*i).A)^((*i).B-(*i).C)).z() < 0  ){
            j = i;
            i++;
            Plist->erase(j);
        } else
            i++;
    }
}

void MPolyObject::drawColoredObj(QPainter &p, QColor &color)
{
    for(auto mp : *Plist) {
        MColoredPolygon(mp,color).drawPoly(p);
    }
}

void MPolyObject::drawColoredObjWithLight(QPainter &p, QColor color, MVector4D & source, MLight model)
{
    MVector4D Norm, PoV(500,-540,5000,1);
    float CP;
    //std::sort(Vect->begin(),Vect->end());
    Plist->sort();
    for(auto mp : *Plist) {
        Norm = ((mp.B-mp.A)^(mp.B-mp.C)).normalize();
        if (Norm.z() >= 0) {
            if(model == Standart)
                CP = calcLight(mp.A, Norm,source);
            else if (model == CookTorrance)
                CP = calcLightCookTorrance(Norm,source,PoV,fresnel((mp.B-PoV).normalize()*Norm,0.8),0.999);
            MColoredPolygon(mp,QColor(CP*color.red(),
                                      CP*color.green(),
                                      CP*color.blue())).drawPoly(p);
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


float calcLight(MVector4D P, MVector4D &N, MVector4D L){
    P.normalize();
    N.normalize();
    float intens = ((P-N).normalize())*((P-L).normalize());
    //std::cout << P-N << std::endl;
    return (intens < 0) ? 0 : intens;
}

float calcLightCookTorrance(MVector4D Norm, MVector4D source, MVector4D PoV, float fresnel, float roughness){
    MVector4D H = (PoV+source).normalize();
    //Norm.normalize();
    //source.normalize();
    PoV.normalize();
    float NdL = max(Norm*source, 0.);
    float NdV = max(Norm*PoV, 0.);
    float NdH = max(Norm*H, 1.0e-7);
    float VdH = max(PoV*H,1.0e-7);

    float geom = 2.0 * NdH/VdH;
    geom = min(1.0, geom*min(NdV,NdL));

    float r_sq = roughness * roughness;
    float NdH_sq = NdH * NdH;
    float NdH_sq_r = 1.0 / (NdH_sq * r_sq);
    float roughness_exp = (NdH_sq - 1.0) * ( NdH_sq_r );
    float nroughness = 0.25 * exp(roughness_exp) * NdH_sq_r / NdH_sq;

    return min(1.0, fresnel * geom * nroughness / (NdV + 1.0e-7));
}

float fresnel(float cosVN, float a){
    float sqra = a*a;
    float acos = a*cosVN;
    float sqracos = acos*acos;
    float one_sqra = 1.0 - sqra;
    float value = acos - sqrt(one_sqra + sqracos);
    value *= value/one_sqra;
    return min(1.0, value*value);

}
