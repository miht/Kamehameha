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

    QGraphicsScene *graphic = new QGraphicsScene(this);
    qDebug() << "*graphics width: " << graphic->width();
    gv = ui->graphicsView;
    gv->sizePolicy ().setHeightForWidth (true);
    graphic -> addPixmap(generatePixmap());
    gv->setScene(graphic);

}

QPixmap MainWindow::generatePixmap() {
    int r = 1;
    int g = 1;
    int b = 0;


    QImage img = QImage(gv->width (), gv->height (), QImage::Format_RGB32);
    qDebug() << "gv width: " << gv->width () << "; h" << gv->height ();
    qDebug() << "gv act width: " << gv->size ().width () << "; h" << gv->size ().height ();

    qDebug() << "img w: " << img.width() << ", h" << img.height();

    for(int i = 0; i < img.width(); i++) {
        for(int j = 0; j < img.height(); j++) {
            img.setPixelColor(i, j, QColor(r, g, b));
        }
    }
    return QPixmap::fromImage(img);
}

MainWindow::~MainWindow()
{
    delete ui;
}
