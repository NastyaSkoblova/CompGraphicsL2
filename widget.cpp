#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    setMouseTracking(true);
    ui->setupUi(this);
    switchshape = Thor;
    hInvisible = false;
    A = 100;
    B = 60;
    D = 30;
    E = 3;
    ui->horizontalSlider->setValue(A);
    ui->horizontalSlider_2->setValue(B);
    ui->horizontalSlider_3->setValue(D);
    changeVisiblyE(false);

}

Widget::~Widget()
{
    delete ui;
}
void Widget::mouseMoveEvent(QMouseEvent *e)
{
    mx = e->x();
    my = e->y();
}


void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    double hheight = height()/2;
    double hwidth = width()/2;
    double sc = (hwidth+hheight)*0.001;
    MVector4D source(-1000,-4000,5000,1);
    double MPI = atan(1)*4;
    MMatrix4D TR(1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1);
    TR.rotateX((my-hheight)*0.0033);
    TR.rotateY((mx-hwidth)*0.0033);
    TR.scale(sc, sc,1);
    TR.transport(hwidth,hheight,0);
    double dt1 = 2*M_PI/D;
    MPolyObject OThor,OPrism,OSphere,OParab;
    MColoredPolygon Light(source,
                          MVector4D(source.x()+10,source.y(),source.z(),1),
                          MVector4D(source.x(),source.y()+10,source.z(),1), QColor("White"));
    drawCoordinateSystem(painter,TR);
    Light.drawPoly(painter);
    switch (switchshape){
    case Thor:
        for(double i = M_PI/2; i > -M_PI*3/2; i-=dt1){
            for(double j = M_PI/2 ; j > -M_PI*3/2; j-=dt1){
                OThor.pushPoly(getThor1Poly(j,i,dt1,TR));
                OThor.pushPoly(getThor2Poly(j,i,dt1,TR));
            }
        }
        if (hInvisible) OThor.hideInvisible();
        if (C) {
            OThor.drawColoredObjWithLight(painter,QColor("Green"),source);
        } else {
            OThor.drawPolyObject(painter);
        }
        break;
    case Sphere:
        for(double i = -MPI; i < MPI; i+=dt1){
            for(double j = MPI; j > 0; j-=dt1){
                OSphere.pushPoly(getSphere1Poly(j,i,dt1,TR));
                OSphere.pushPoly(getSphere2Poly(j,i,dt1,TR));
            }
        }
        if (hInvisible) OSphere.hideInvisible();
        if (C) {
            OSphere.drawColoredObjWithLight(painter,QColor("Green"),source);
            //OSphere.drawColoredObj(painter,QColor(255,0,0));
        } else {
            OSphere.drawPolyObject(painter);
        }
        break;
    case Parabaloid:
        A *= 0.1;
        for(double j = 0, dt2 = B*0.3/D; j < B*0.3; j+=dt2){
            for(double i = 0; i < 2*M_PI; i+=dt1){
                OParab.pushPoly(getParabPoly(i,j,dt1,dt2,TR));
            }
        }
        A *= 10;
        if (hInvisible) OParab.hideInvisible();
        if (C) {
            OParab.drawColoredObjWithLight(painter,QColor("Green"),source);
        } else {
            OParab.drawPolyObject(painter);
        }
        break;
    case Prism:
        for(double j = 0, dt = 2*M_PI/E; j < 2*M_PI; j+=dt){
            OPrism.pushPoly(getPrism1Poly(j, dt, TR));
            OPrism.pushPoly(getPrism2Poly(j, dt, TR));
            OPrism.pushPoly(getPrism3Poly(j, dt ,A, TR));
            OPrism.pushPoly(getPrism4Poly(j, dt ,-A, TR));
        }
        if (hInvisible) OPrism.hideInvisible();
        if (C) {
            OPrism.drawColoredObjWithLight(painter,QColor("Green"),source);
        } else {
            OPrism.drawPolyObject(painter);
        }

    }

    update();
}

void Widget::drawCoordinateSystem(QPainter &p, MMatrix4D & M){
    MVector4D center(1,1,1,1);
    MVector4D OX(200,1,1,1);
    MVector4D OY(1,-200,1,1);
    MVector4D OZ(1,1,200,1);
    center = M*center;
    OX = M*OX;
    OY = M*OY;
    OZ = M*OZ;
    p.setPen(QColor(255,0,0));
    p.drawLine(center.x(),center.y(),OX.x(),OX.y());
    p.setPen(QColor(0,255,0));
    p.drawLine(center.x(),center.y(),OY.x(),OY.y());
    p.setPen(QColor(0,0,255));
    p.drawLine(center.x(),center.y(),OZ.x(),OZ.y());
    p.setPen(QColor(0,0,0));

}

MPolygon Widget::getPrism1Poly(double a,double b, const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sab = sin(a+b);
    double cab = cos(a+b);
    return MPolygon(M*MVector4D(B*ca,A,B*sa,1),
                    M*MVector4D(B*ca,-A,B*sa,1),
                    M*MVector4D(B*cab,-A,B*sab,1));
}
MPolygon Widget::getPrism2Poly(double a,double b, const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sab = sin(a+b);
    double cab = cos(a+b);
    return MPolygon(M*MVector4D(B*ca,A,B*sa,1),
                    M*MVector4D(B*cab,-A,B*sab,1),
                    M*MVector4D(B*cab,A,B*sab,1));
}
MPolygon Widget::getPrism3Poly(double a,double b, double c,const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sab = sin(a+b);
    double cab = cos(a+b);
    return MPolygon(M*MVector4D(B*cab,c,B*sab,1),
                    M*MVector4D(0,c,0,1),
                    M*MVector4D(B*ca,c,B*sa,1));
}
MPolygon Widget::getPrism4Poly(double a,double b, double c,const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sab = sin(a+b);
    double cab = cos(a+b);
    return MPolygon(M*MVector4D(B*ca,c,B*sa,1),
                    M*MVector4D(0,c,0,1),
                    M*MVector4D(B*cab,c,B*sab,1));
}
MPolygon Widget::getThor1Poly(double a, double b, double c, const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sac = sin(a+c);
    double cac = cos(a+c);
    double sb = sin(b);
    double cb = cos(b);
    double sbc = sin(b+c);
    double cbc = cos(b+c);
    return MPolygon(M*MVector4D((A+B* ca)*cb,(A+B*ca)*sb,B*sa,1),
                    M*MVector4D((A+B*cac)*cb,(A+B*cac)*sb,B*sac,1),
                    M*MVector4D((A+B*ca)*cbc,(A+B*ca)*sbc,B*sa,1));
}
MPolygon Widget::getThor2Poly(double a, double b, double c, const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sac = sin(a+c);
    double cac = cos(a+c);
    double sb = sin(b);
    double cb = cos(b);
    double sbc = sin(b+c);
    double cbc = cos(b+c);
    return MPolygon(M*MVector4D((A+B*cac)*cb,(A+B*cac)*sb,B*sac,1),
                    M*MVector4D((A+B* cac)*cbc,(A+B*cac)*sbc,B*sac,1),
                    M*MVector4D((A+B*ca)*cbc,(A+B*ca)*sbc,B*sa,1));
}

MPolygon Widget::getSphere1Poly(double a, double b, double c, const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sac = sin(a+c);
    double cac = cos(a+c);
    double sb = sin(b);
    double cb = cos(b);
    double sbc = sin(b+c);
    double cbc = cos(b+c);
    return MPolygon(M*MVector4D(A*sa*cb,A*sa*sb,A*ca,1),
                    M*MVector4D(A*sa*cbc,A*sa*sbc,A*ca,1),
                    M*MVector4D(A*sac*cb,A*sac*sb,A*cac,1));
}
MPolygon Widget::getSphere2Poly(double a, double b, double c, const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sac = sin(a+c);
    double cac = cos(a+c);
    double sb = sin(b);
    double cb = cos(b);
    double sbc = sin(b+c);
    double cbc = cos(b+c);
    return MPolygon(M*MVector4D(A*sa*cbc,A*sa*sbc,A*ca,1),
                    M*MVector4D(A*sac*cbc,A*sac*sbc,A*cac,1),
                    M*MVector4D(A*sac*cb,A*sac*sb,A*cac,1));
}
MPolygon Widget::getParabPoly(double a, double b, double c, double e, const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sac = sin(a+c);
    double cac = cos(a+c);
    return MPolygon(M*MVector4D(A*(b)*ca,A*(b)*sa,b*b,1),
                    M*MVector4D(A*(b)*cac,A*(b)*sac,b*b,1),
                    M*MVector4D(A*(b+e)*ca,A*(b+e)*sa,(b+e)*(b+e),1));
}

void Widget::on_comboBox_activated(const QString &arg1)
{

    if(arg1 == "Thor"){
        switchshape = Thor;
        changeVisiblyB(true);
        changeVisiblyD(true);
        changeVisiblyE(false);
    } else if(arg1 == "Sphere") {
        switchshape = Sphere;
        changeVisiblyB(false);
        changeVisiblyD(true);
        changeVisiblyE(false);
    } else if(arg1 == "Parabaloid") {
        switchshape = Parabaloid;
        changeVisiblyB(true);
        changeVisiblyD(true);
        changeVisiblyE(false);
    } else if(arg1 == "Prism"){
        switchshape = Prism;
        changeVisiblyB(true);
        changeVisiblyD(false);
        changeVisiblyE(true);
    }
    update();
}

void Widget::changeVisiblyB(bool bl){
    if(bl){
        ui->label_2->show();
        ui->horizontalSlider_2->show();
    } else {
        ui->label_2->hide();
        ui->horizontalSlider_2->hide();
    }
}

void Widget::changeVisiblyD(bool bl){
    if(bl){
        ui->label_3->show();
        ui->horizontalSlider_3->show();
    } else {
        ui->label_3->hide();
        ui->horizontalSlider_3->hide();
    }
}

void Widget::changeVisiblyE(bool bl){
    if(bl){
        ui->label_4->show();
        ui->spinBox->show();
    } else {
        ui->label_4->hide();
        ui->spinBox->hide();
    }
}


void Widget::on_pushButton_clicked()
{
    close();
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    A = value;
}

void Widget::on_horizontalSlider_2_valueChanged(int value)
{
    B = value;
}

void Widget::on_horizontalSlider_3_valueChanged(int value)
{
    D = value;
}
void Widget::on_horizontalSlider_2_destroyed(){}

//void Widget::on_radioButton_clicked(bool ){}

void Widget::on_spinBox_valueChanged(int arg1)
{
    E = arg1;
}

//void Widget::on_radioButton_2_clicked(bool ){ }

void Widget::on_checkBox_clicked(bool checked)
{
    hInvisible = checked;
}

void Widget::on_checkBox_2_clicked(bool checked)
{
    C = checked;
}
