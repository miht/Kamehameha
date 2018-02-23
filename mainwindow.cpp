#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "model3d.h"
#include "objparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Model3D mod = ObjParser::parse("/Users/leifthysellsundqvist/Library/Mobile Documents/com~apple~CloudDocs/Skolan/Y4/VisGraf18/ProjectTest/cube");
//    for (Triangle3D tri : mod.triangles) {
//        //qDebug() << tri;
//    }
//    for(QString m_key : mod.materials.keys ()) {
//        qDebug() << m_key << ": " << mod.materials.value (m_key);
//    }

    graphic = new QGraphicsScene(this);
    graphic -> addPixmap(generatePixmap());
    ui->graphicsView->setScene(graphic);
}

MainWindow::~MainWindow()
{
    delete ui;
}
