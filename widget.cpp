#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    setMouseTracking(true);
    ui->setupUi(this);
    switchshape = 1;

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
    MPolygon P1;
    double sc = (width()+height())*0.001/2;
    MMatrix4D TR(1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1);
    TR.rotateX((my-height()/2)*0.0033);
    TR.rotateY((mx-width()/2)*0.0033);
    TR.scale(sc, sc,1);
    TR.transport(width()/2,height()/2,1);


    R=300,r=30;
    double dt1 = 2*M_PI/100;
    drawCoordinateSystem(painter,TR);
    if (switchshape == 1) {
        ui->label->hide();
        for(double i = 0; i < 2*M_PI; i+=dt1){
            for(double j = 0; j < 2*M_PI; j+=dt1){
                P1 = getThorPoly(i,j,dt1,TR);
                P1.drawPoly(painter);

            }
        }
    } else if (switchshape == 2){
        ui->label->show();
        for(double j = 0; j < 50; j+=3){
            for(double i = 0; i < 2*M_PI; i+=dt1){
                P1 = getParabPoly(i,j,dt1,3,TR);
                P1.drawPoly(painter);
            }
        }
    } else if (switchshape == 3){
        for(double i = 0; i < 2*M_PI; i+=dt1){
            for(double j = 0; j < 2*M_PI; j+=dt1){
                P1 = getSpherePoly(i,j,dt1,TR);
                P1.drawPoly(painter);

            }
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

MPolygon Widget::getThorPoly(double a, double b, double c, const MMatrix4D &M){
    return MPolygon(M*MVector4D((R+r*cos(a))*cos(b),(R+r*cos(a))*sin(b),r*sin(a),1),
                    M*MVector4D((R+r*cos(a+c))*cos(b),(R+r*cos(a+c))*sin(b),r*sin(a+c),1),
                    M*MVector4D((R+r*cos(a))*cos(b+c),(R+r*cos(a))*sin(b+c),r*sin(a),1));
}

MPolygon Widget::getSpherePoly(double a, double b, double c, const MMatrix4D &M){
    return MPolygon(M*MVector4D(R*sin(a)*cos(b),R*sin(a)*sin(b),R*cos(a),1),
                    M*MVector4D(R*sin(a+c)*cos(b),R*sin(a+c)*sin(b),R*cos(a+c),1),
                    M*MVector4D(R*sin(a)*cos(b+c),R*sin(a)*sin(b+c),R*cos(a),1));
}
MPolygon Widget::getParabPoly(double a, double b, double c, double e, const MMatrix4D &M){
    return MPolygon(M*MVector4D(r*(b)*cos(a),r*(b)*sin(a),b*b,1),
                    M*MVector4D(r*(b)*cos(a+c),r*(b)*sin(a+c),b*b,1),
                    M*MVector4D(r*(b+e)*cos(a),r*(b+e)*sin(a),(b+e)*(b+e),1));
}

void Widget::on_comboBox_activated(const QString &arg1)
{

    if(arg1 == "Thor"){
        switchshape = 1;
    } else if(arg1 == "Parabaloid") {
        switchshape = 2;
    } else if(arg1 == "Sphere") {
        switchshape = 3;
    }
    update();
}
