#include <QApplication>
#include <QDebug>
#include <iostream>
#include "kamehameha.h"
#include "model.h"
#include "face.h"
#include "objparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kamehameha w;
    w.show();

    return a.exec();
}
