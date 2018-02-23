#include <QApplication>
#include <QDebug>
#include <iostream>
#include "mainwindow.h"
#include "model3d.h"
#include "triangle3d.h"
#include "objparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
