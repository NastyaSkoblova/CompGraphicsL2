#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "mpolygon.h"

enum MShape{Thor = 1, Sphere, Parabaloid, Prism, SWatch};
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    float A,B,D, mx, my, MPI;
    float LX,LY,LZ;
    int E;
    MShape switchshape;
    MLight model;
    bool hInvisible,C;
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);
    MPolygon getThor1Poly(float a, float b, float c, const MMatrix4D &M);
    MPolygon getThor2Poly(float a, float b, float c, const MMatrix4D &M);
    MPolygon getSphere1Poly(float a, float b, float c, const MMatrix4D &M);
    MPolygon getSphere2Poly(float a, float b, float c, const MMatrix4D &M);
    void drawCoordinateSystem(QPainter &p, MMatrix4D &M);
    MPolygon getParab1Poly(float a, float b, float c, float e, const MMatrix4D &M);
    MPolygon getParab2Poly(float a, float b, float c, float e, const MMatrix4D &M);
    MPolygon getPrism1Poly(float a, float b, const MMatrix4D &M);
    MPolygon getPrism2Poly(float a, float b, const MMatrix4D &M);
    MPolygon getPrism3Poly(float a, float b, float c, const MMatrix4D &M);
    MPolygon getPrism4Poly(float a, float b, float c, const MMatrix4D &M);
    MPolygon getSWatch1Poly(float a, float b, float c, const MMatrix4D &M);
    MPolygon getSWatch2Poly(float a, float b, float c, const MMatrix4D &M);
    void changeVisiblyE(bool bl);
    void changeVisiblyD(bool bl);
    void changeVisiblyB(bool bl);
    void changeVisiblySoL(bool bl);

private:
    Ui::Widget *ui;
protected:
    void mouseMoveEvent(QMouseEvent *);
private slots:
    void on_comboBox_activated(const QString &arg1);
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_spinBox_valueChanged(int arg1);
    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_horizontalSlider_4_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);
    void on_horizontalSlider_5_valueChanged(int value);
    void on_comboBox_2_activated(const QString &arg1);
};

#endif // WIDGET_H
