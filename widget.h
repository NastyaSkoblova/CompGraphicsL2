#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "mpolygon.h"

enum MShape{Thor = 1, Sphere, Parabaloid, Prism};
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    double A,B,D, mx, my;
    double LX,LY,LZ;
    int E;
    MShape switchshape;
    bool hInvisible,C;
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);
    MPolygon getThor1Poly(double a, double b, double c, const MMatrix4D &M);
    MPolygon getThor2Poly(double a, double b, double c, const MMatrix4D &M);
    MPolygon getSphere1Poly(double a, double b, double c, const MMatrix4D &M);
    MPolygon getSphere2Poly(double a, double b, double c, const MMatrix4D &M);
    void drawCoordinateSystem(QPainter &p, MMatrix4D &M);
    MPolygon getParabPoly(double a, double b, double c, double e, const MMatrix4D &M);
    MPolygon getPrism1Poly(double a, double b, const MMatrix4D &M);
    MPolygon getPrism2Poly(double a, double b, const MMatrix4D &M);
    MPolygon getPrism3Poly(double a, double b, double c, const MMatrix4D &M);
    MPolygon getPrism4Poly(double a, double b, double c, const MMatrix4D &M);
    void changeVisiblyE(bool bl);
    void changeVisiblyD(bool bl);
    void changeVisiblyB(bool bl);
private:
    Ui::Widget *ui;
protected:
    void mouseMoveEvent(QMouseEvent *);
private slots:
    void on_comboBox_activated(const QString &arg1);
    void on_pushButton_clicked();
    //void on_horizontalSlider_2_destroyed();
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    //void on_radioButton_clicked(bool);
    void on_spinBox_valueChanged(int arg1);
    void on_radioButton_2_clicked(bool);
    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_horizontalSlider_4_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);
    void on_horizontalSlider_5_valueChanged(int value);
};

#endif // WIDGET_H
