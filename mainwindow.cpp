#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include "model3d.h"
#include "objparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Model3D mod = ObjParser::parse("/Users/leifthysellsundqvist/Library/Mobile Documents/com~apple~CloudDocs/Skolan/Y4/VisGraf18/ProjectTest/cube.obj");
    qDebug() << mod.triangles.size ();
    for (Triangle3D tri : mod.triangles) {
        qDebug() << tri;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
