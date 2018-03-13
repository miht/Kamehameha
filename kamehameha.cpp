#include "kamehameha.h"

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
    graphicsScene->setSceneRect (0, 0, 300, 300);
    graphicsView = ui->graphicsView;
}

void Kamehameha::on_renderButton_clicked()
{
    Scene scene = Scene();
    // TODO get resolution from user input
    RayTracer rt = RayTracer(graphicsView->width (), graphicsView->height (), scene);
    Wireframer wf = Wireframer(graphicsView->width (), graphicsView->height (), scene);

    // configure progress bar
    ui_renderProgressBar->setMinimum(0);
    ui_renderProgressBar->setMaximum(rt.w * rt.h);

    QFile file("out.ppm"); // TODO: file selection
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
//        rt.trace(out, ui_renderProgressBar, pixmap);

        QFutureSynchronizer<QImage> synchronizer;

        //TEST GENERATE RAYTRACED IMAGE

//        for(int i = 0; i < rt.w; i+= rt.w/4) {
//            for(int j = 0; j < rt.h; j += rt.h/4) {
//                        synchronizer.addFuture (QtConcurrent::run(rt, &RayTracer::generate,
//                                                                 ui_renderProgressBar,
//                                                                 i, j, rt.w/4, rt.h/4));
//            }
//        }

//        synchronizer.waitForFinished ();
//        for(QImage img : synchronizer.futures ()) {
//            QGraphicsPixmapItem* item = graphicsScene->addPixmap(QPixmap::fromImage (img));
//            item->setPos(img.offset ());
//        }

        //TEST GENERATE WIREFRAME IMAGE
        graphicsScene->addPixmap (QPixmap::fromImage(wf.generate (ui_renderProgressBar, 0, 0, rt.w, rt.h)));

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
