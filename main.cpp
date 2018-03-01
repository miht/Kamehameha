#include <QApplication>
#include <QDebug>
#include <iostream>
#include "kamehameha.h"
#include "model3d.h"
#include "triangle.h"
#include "objparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kamehameha w;
    w.show();

    return a.exec();
}
