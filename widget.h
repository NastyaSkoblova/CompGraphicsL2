#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "mpolygon.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    double R,r, mx, my;
    int switchshape;
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);
    MPolygon getThorPoly(double a, double b, double c, const MMatrix4D &M);
    MPolygon getSpherePoly(double a, double b, double c, const MMatrix4D &M);
    void drawCoordinateSystem(QPainter &p, MMatrix4D &M);
    MPolygon getParabPoly(double a, double b, double c, double e, const MMatrix4D &M);
private:
    Ui::Widget *ui;
protected:
    void mouseMoveEvent(QMouseEvent *);
private slots:
    void on_comboBox_activated(const QString &arg1);
};

#endif // WIDGET_H
