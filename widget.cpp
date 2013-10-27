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
    ui->horizontalSlider->setValue(A);
    ui->horizontalSlider_2->setValue(B);
    ui->horizontalSlider_3->setValue(D);

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
    MMatrix4D TR(1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1);
    TR.rotateX((my-hheight)*0.0033);
    TR.rotateY((mx-hwidth)*0.0033);
    TR.scale(sc, sc,1);
    TR.transport(hwidth,hheight,1);
    double dt1 = 2*M_PI/D;
    MPolyObject OThor,OPrism,OSphere,OParab;
    drawCoordinateSystem(painter,TR);
    switch (switchshape){
    case Thor:
        for(double i = 0; i < 2*M_PI; i+=dt1){
            for(double j = 0; j < 2*M_PI; j+=dt1){
                OThor.pushPoly(getThorPoly(i,j,dt1,TR));
            }
        }
        if (hInvisible) OThor.hideInvisible();
        OThor.drawPolyObject(painter);
        break;
    case Sphere:
        for(double i = 0; i < 2*M_PI; i+=dt1){
            for(double j = M_PI; j > 0; j-=dt1){
                OSphere.pushPoly(getSpherePoly(j,i,dt1,TR));
            }
        }
        if (hInvisible) OSphere.hideInvisible();
        OSphere.drawPolyObject(painter);
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
        OParab.drawPolyObject(painter);
        break;
    case Prism:
        for(double j = 0, dt = 2*M_PI/3; j < 2*M_PI; j+=dt){
            OPrism.pushPoly(getPrism1Poly(j, dt, TR));
            OPrism.pushPoly(getPrism2Poly(j, dt, TR));
            OPrism.pushPoly(getPrism3Poly(j, dt ,A, TR));
            OPrism.pushPoly(getPrism4Poly(j, dt ,-A, TR));
        }
        if (hInvisible) OPrism.hideInvisible();
        OPrism.drawPolyObject(painter);

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
MPolygon Widget::getThorPoly(double a, double b, double c, const MMatrix4D &M){
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

MPolygon Widget::getSpherePoly(double a, double b, double c, const MMatrix4D &M){
    double sa = sin(a);
    double ca = cos(a);
    double sac = sin(a+c);
    double cac = cos(a+c);
    double sb = sin(b);
    double cb = cos(b);
    double sbc = sin(b+c);
    double cbc = cos(b+c);
    return MPolygon(M*MVector4D(A*sa*cb,A*sa*sb,A*ca,1),
                    M*MVector4D(A*sac*cb,A*sac*sb,A*cac,1),
                    M*MVector4D(A*sa*cbc,A*sa*sbc,A*ca,1));
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
        ui->label_3->show();
        ui->horizontalSlider_3->show();
        ui->label_2->show();
        ui->horizontalSlider_2->show();
    } else if(arg1 == "Sphere") {
        ui->label_3->show();
        ui->horizontalSlider_3->show();
        ui->label_2->hide();
        ui->horizontalSlider_2->hide();
        switchshape = Sphere;
    } else if(arg1 == "Parabaloid") {
        ui->label_3->show();
        ui->horizontalSlider_3->show();
        ui->label_2->show();
        ui->horizontalSlider_2->show();
        switchshape = Parabaloid;
    } else if(arg1 == "Prism"){
        ui->label_2->show();
        ui->horizontalSlider_2->show();
        ui->label_3->hide();
        ui->horizontalSlider_3->hide();
        switchshape = Prism;
    }
    update();
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

void Widget::on_radioButton_clicked(bool checked)
{
    hInvisible = checked;
}
