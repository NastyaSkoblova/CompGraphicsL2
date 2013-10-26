#include "widget.h"
#include "MMatrix.h"
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    MMatrix4D M(1,2,3,0,5,6,7,0,9,10,11,0,0,0,0,1), T(1,0,0,1,
                                                      0,1,0,0,
                                                      0,0,1,0,
                                                      0,0,0,1);
    MVector4D V(3,2,5,1);
    //std::cout<<M<<std::endl;
    //M.rotateX(.002).rotateY(.001);
    //           std::cout <<M<<std::endl;
    T.transport(2,2,2);
    V = T*V;
    std::cout << V <<std::endl << T;

    return a.exec();
}
