#include "mainwindow.h"
#include "model3d.h"
#include "triangle3d.h"
#include "objparser.h"
#include <QApplication>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Model3D mod = ObjParser::parse("../ProjectTest/cube.obj");
    for(Triangle3D tri : mod.triangles) {
        qDebug() << "Triangle " << "tri";
    }

    return a.exec();
}
