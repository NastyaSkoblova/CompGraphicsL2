#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#define sqr(x) (x*x)

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    setMouseTracking(true);
    ui->setupUi(this);
    switchshape = Thor;
    model = BlinnFong;
    MPI = atan(1)*4;
    hInvisible = false;
    C = false;
    A = 100;
    B = 60;
    D = 30;
    E = 3;
    LX = -1000;
    LY = -2000;
    LZ = 7000;
    ui->horizontalSlider->setValue(A);
    ui->horizontalSlider_2->setValue(B);
    ui->horizontalSlider_3->setValue(D);
    ui->horizontalSlider_4->setValue(LX);
    ui->horizontalSlider_5->setValue(LZ);
    ui->horizontalSlider_6->setValue(LY);
    changeVisiblyE(false);
    changeVisiblySoL(false);

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
    float hheight = height()/2;
    float hwidth = width()/2;
    float sc = (hwidth+hheight)*0.001;
    QColor colorA(ui->spinBox_2->value(),ui->spinBox_3->value(),ui->spinBox_4->value());
    QColor colorD(ui->spinBox_5->value(),ui->spinBox_6->value(),ui->spinBox_7->value());
    QColor colorS(ui->spinBox_8->value(),ui->spinBox_9->value(),ui->spinBox_10->value());
    MVector4D source(LX,LY,LZ,1);
    MMatrix4D TR(1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1);
    TR.rotateX((my-hheight)*0.0033);
    TR.rotateY((mx-hwidth)*0.0033);
    TR.scale(sc, sc,1);
    TR.transport(hwidth,hheight,0);
    float dt1 = 2*M_PI/(D+20);
    MPolyObject OFigure;
    /*MColoredPolygon Light(source,
                          MVector4D(source.x()+10,source.y(),source.z(),1),
                          MVector4D(source.x(),source.y()+10,source.z(),1), QColor("White"));*/
    drawCoordinateSystem(painter,TR);
    //Light.drawPoly(painter);
    switch (switchshape){
    case Thor:
        for(float i = MPI/2, lim = -MPI*3/2; i > lim; i-=dt1){
            for(float j = MPI/2 ; j > lim; j-=dt1){
                OFigure.pushPoly(getThorPoly(j,i,dt1,TR));
                OFigure.pushPoly(getThorPoly(j+dt1,i+dt1,-dt1,TR));
            }
        }
        break;
    case Sphere:
        for(float i = -MPI; i < MPI+0.00001; i+=dt1){
            for(float j = MPI; j > -0.00001; j-=dt1){
                OFigure.pushPoly(getSpherePoly(j,i,dt1,TR));
                OFigure.pushPoly(getSpherePoly(j+dt1,i+dt1,-dt1,TR));
            }
        }
        break;
    case Parabaloid:
        A *= 0.1;
        for(float j = 0, lim1 = B*0.3, dt2 = lim1/D; j < lim1; j+=dt2){
            for(float i = 0, lim2 = 2*MPI; i < lim2; i+=dt1){
                OFigure.pushPoly(getParabPoly(i,j,dt1,dt2,TR));
                OFigure.pushPoly(getParabPoly(i+dt1,j+dt2,-dt1,-dt2,TR));
            }
        }
        A *= 10;
        break;
    case Prism:
        for(float j = 0, dt = 2*MPI/E,lim = 2*MPI; j < lim; j+=dt){
            OFigure.pushPoly(getPrism1Poly(j, dt, TR));
            OFigure.pushPoly(getPrism2Poly(j, dt, TR));
            OFigure.pushPoly(getPrism3Poly(j, dt ,A, TR));
            OFigure.pushPoly(getPrism4Poly(j, dt ,-A, TR));
        }
        break;
     case SWatch:
        A*=3;
        for(float i = 0, lim = 2*MPI; i < lim; i+=dt1 ){
            for(float j = 0; j < lim; j+=dt1 ){
                OFigure.pushPoly(MPolygon(getSWatchPoly(i,j,dt1,TR)));
                OFigure.pushPoly(MPolygon(getSWatchPoly(i+dt1,j+dt1,-dt1,TR)));
            }
        }
        A/=3;
        break;
    }
    if (hInvisible) OFigure.hideInvisible();
    if (C) {
        OFigure.drawColoredObjWithLight(painter,colorA,colorD,colorS,source, model,ui->doubleSpinBox->value(), ui->doubleSpinBox_2->value());//rewrite *func
    } else {
        OFigure.drawPolyObject(painter);
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
MPolygon Widget::getSWatchPoly(float a,float b, float c, const MMatrix4D &M){
    float sa = sin(a);
    float ca = cos(a);
    float sac = sin(a+c);
    float cac = cos(a+c);
    float sb = sin(b);
    float cb = cos(b);
    float sbc = sin(b+c);
    float cbc = cos(b+c);
    return MPolygon(M*MVector4D(A*ca*sb*sa,A*sa*sb,A*cb*sb*sa,1),
                    M*MVector4D(A*ca*sbc*sa,A*sa*sbc,A*cbc*sbc*sa,1),
                    M*MVector4D(A*cac*sb*sac,A*sac*sb,A*cb*sb*sac,1));
}

MPolygon Widget::getPrism1Poly(float a,float b, const MMatrix4D &M){
    float sa = sin(a);
    float ca = cos(a);
    float sab = sin(a+b);
    float cab = cos(a+b);
    return MPolygon(M*MVector4D(B*ca,A,B*sa,1),
                    M*MVector4D(B*ca,-A,B*sa,1),
                    M*MVector4D(B*cab,-A,B*sab,1));
}
MPolygon Widget::getPrism2Poly(float a,float b, const MMatrix4D &M){
    float sa = sin(a);
    float ca = cos(a);
    float sab = sin(a+b);
    float cab = cos(a+b);
    return MPolygon(M*MVector4D(B*ca,A,B*sa,1),
                    M*MVector4D(B*cab,-A,B*sab,1),
                    M*MVector4D(B*cab,A,B*sab,1));
}
MPolygon Widget::getPrism3Poly(float a,float b, float c,const MMatrix4D &M){
    float sa = sin(a);
    float ca = cos(a);
    float sab = sin(a+b);
    float cab = cos(a+b);
    return MPolygon(M*MVector4D(B*cab,c,B*sab,1),
                    M*MVector4D(0,c,0,1),
                    M*MVector4D(B*ca,c,B*sa,1));
}
MPolygon Widget::getPrism4Poly(float a,float b, float c,const MMatrix4D &M){
    float sa = sin(a);
    float ca = cos(a);
    float sab = sin(a+b);
    float cab = cos(a+b);
    return MPolygon(M*MVector4D(B*ca,c,B*sa,1),
                    M*MVector4D(0,c,0,1),
                    M*MVector4D(B*cab,c,B*sab,1));
}
MPolygon Widget::getThorPoly(float a, float b, float c, const MMatrix4D &M){
    float sa = sin(a);
    float ca = cos(a);
    float sac = sin(a+c);
    float cac = cos(a+c);
    float sb = sin(b);
    float cb = cos(b);
    float sbc = sin(b+c);
    float cbc = cos(b+c);
    return MPolygon(M*MVector4D((A+B*ca)*cb,(A+B*ca)*sb,B*sa,1),
                    M*MVector4D((A+B*cac)*cb,(A+B*cac)*sb,B*sac,1),
                    M*MVector4D((A+B*ca)*cbc,(A+B*ca)*sbc,B*sa,1));
}

MPolygon Widget::getSpherePoly(float a, float b, float c, const MMatrix4D &M){
    float sa = sin(a);
    float ca = cos(a);
    float sac = sin(a+c);
    float cac = cos(a+c);
    float sb = sin(b);
    float cb = cos(b);
    float sbc = sin(b+c);
    float cbc = cos(b+c);
    return MPolygon(M*MVector4D(A*sa*cb,A*sa*sb,A*ca,1),
                    M*MVector4D(A*sa*cbc,A*sa*sbc,A*ca,1),
                    M*MVector4D(A*sac*cb,A*sac*sb,A*cac,1));
}

MPolygon Widget::getParabPoly(float a, float b, float c, float e, const MMatrix4D &M){
    float sa = sin(a);
    float ca = cos(a);
    float sac = sin(a+c);
    float cac = cos(a+c);
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
    }else if(arg1 == "Sand Watch"){
        switchshape = SWatch;
        changeVisiblyB(false);
        changeVisiblyD(true);
        changeVisiblyE(false);
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

void Widget::changeVisiblySoL(bool bl)
{
    if(bl){
        ui->horizontalSlider_4->show();
        ui->horizontalSlider_5->show();
        ui->horizontalSlider_6->show();
        ui->comboBox_2->show();
        ui->label_5->show();
        ui->label_6->show();
        ui->label_7->show();
        ui->label_8->show();
        ui->label_9->show();
    } else {
        ui->horizontalSlider_4->hide();
        ui->horizontalSlider_5->hide();
        ui->horizontalSlider_6->hide();
        ui->comboBox_2->hide();
        ui->label_5->hide();
        ui->label_6->hide();
        ui->label_7->hide();
        ui->label_8->hide();
        ui->label_9->hide();
    }
    changeVisiblyRGB(bl);
}

void Widget::changeVisiblyRGB(bool bl)
{
    if(bl){
        ui->label_10->show();
        ui->spinBox_2->show();
        ui->spinBox_3->show();
        ui->spinBox_4->show();
        if (model == CookTorrance) changeVisiblyCoTr(true);
        if (model == BlinnFong) changeVisiblyBlFg(true);
    } else {
        ui->label_10->hide();
        ui->spinBox_2->hide();
        ui->spinBox_3->hide();
        ui->spinBox_4->hide();
        changeVisiblyCoTr(false);
        changeVisiblyBlFg(false);
    }


}

void Widget::changeVisiblyCoTr(bool bl)
{
    if (bl) {
        ui->label_11->show();
        ui->label_12->show();
        ui->doubleSpinBox->show();
        ui->doubleSpinBox_2->show();
    } else {
        ui->label_11->hide();
        ui->label_12->hide();
        ui->doubleSpinBox->hide();
        ui->doubleSpinBox_2->hide();
    }
}

void Widget::changeVisiblyBlFg(bool bl)
{
    if (bl) {
        ui->spinBox_5->show();
        ui->spinBox_6->show();
        ui->spinBox_7->show();
        ui->spinBox_8->show();
        ui->spinBox_9->show();
        ui->spinBox_10->show();
    } else {
        ui->spinBox_5->hide();
        ui->spinBox_6->hide();
        ui->spinBox_7->hide();
        ui->spinBox_8->hide();
        ui->spinBox_9->hide();
        ui->spinBox_10->hide();
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
//void Widget::on_horizontalSlider_2_destroyed(){}

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
    changeVisiblySoL(checked);
}

void Widget::on_horizontalSlider_4_valueChanged(int value)
{
    LX = value;
}

void Widget::on_horizontalSlider_6_valueChanged(int value)
{
    LY = value;
}

void Widget::on_horizontalSlider_5_valueChanged(int value)
{
    LZ = value;
}

void Widget::on_comboBox_2_activated(const QString &arg1)
{
    if (arg1 == "CookTorrance"){
        model = CookTorrance;
        changeVisiblyCoTr(true);
        changeVisiblyBlFg(false);
    } else if (arg1 == "BlinnFong") {
        model = BlinnFong;
        changeVisiblyCoTr(false);
        changeVisiblyBlFg(true);
    }
}

