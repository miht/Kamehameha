#include "kamehameha.h"
#include "ui_mainwindow.h"
#include "raytracer.h"

#include <QFile>
#include <QTextStream>

Kamehameha::Kamehameha(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    width = height = 400;

    // get widgets
    ui_renderButton = findChild<QPushButton*>("renderButton");
    ui_cancelButton = findChild<QPushButton*>("cancelButton");
    ui_logBrowser = findChild<QTextBrowser*>("logBrowser");
    ui_renderProgressBar = findChild<QProgressBar*>("renderProgressBar");

    ui_renderProgressBar->setValue(0);

    //Initialize graphic elements
    graphicsScene = new QGraphicsScene(this);
    graphicsScene->setSceneRect (0, 0, width, width);
    graphicsView = ui->graphicsView;

}

void Kamehameha::on_renderButton_clicked()
{
    // TODO get resolution from user input
    RayTracer rt = RayTracer(width, height, Light(Vector3D(-2,-3,2)));

    // configure progress bar
    ui_renderProgressBar->setMinimum(0);
    ui_renderProgressBar->setMaximum(rt.w * rt.h);

    QFile file("out.ppm"); // TODO: file selection
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
//        rt.trace(out, ui_renderProgressBar, pixmap);

        QFutureSynchronizer<QImage> synchronizer;

        for(int i = 0; i < rt.w; i+= rt.w/4) {
            for(int j = 0; j < rt.h; j += rt.h/4) {
                        synchronizer.addFuture (QtConcurrent::run(rt, &RayTracer::generate,
                                                                 ui_renderProgressBar,
                                                                 i, j, rt.w/4, rt.h/4));
            }
        }
//        synchronizer.addFuture (QtConcurrent::run(rt, &RayTracer::trace,
//                                                 ui_renderProgressBar,
//                                                 0, 0, rt.w/2, rt.h/2));
//        synchronizer.addFuture (QtConcurrent::run(rt, &RayTracer::trace,
//                                                 ui_renderProgressBar,
//                                                 rt.w/2, 0, rt.w/2, rt.h/2));

        synchronizer.waitForFinished ();
        for(QImage img : synchronizer.futures ()) {
            QGraphicsPixmapItem* item = graphicsScene->addPixmap(QPixmap::fromImage (img));
            item->setPos(img.offset ());
        }
//        future.waitForFinished ();

//        graphicsScene->addPixmap (QPixmap::fromImage (img));
        graphicsView->setScene(graphicsScene);
    }
    file.close();
}

void Kamehameha::on_cancelButton_clicked()
{
    QApplication::quit(); // placeholder

}

Kamehameha::~Kamehameha()
{
    delete ui;
}
