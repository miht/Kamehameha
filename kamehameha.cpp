#include "kamehameha.h"

Kamehameha::Kamehameha(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // get widgets
    ui_renderButton = findChild<QPushButton*>("renderButton");
    ui_cancelButton = findChild<QPushButton*>("cancelButton");
    ui_logBrowser = findChild<QTextBrowser*>("logBrowser");
    ui_renderProgressBar = findChild<QProgressBar*>("renderProgressBar");

    ui_renderProgressBar->setValue(0);

    //Initialize graphic elements and configure the graphics view
    graphicsScene = new QGraphicsScene(this);
    graphicsView = ui->graphicsView;
    width = graphicsView->width ();
    height = graphicsView->height ();
    graphicsView->setSceneRect (0,0,width,height);

    // configure progress bar
    ui_renderProgressBar->setMinimum(0);
    ui_renderProgressBar->setMaximum(width * height);

    scene = new Scene(0.1, Color(0,0,0));
    scene->lights.push_back(Light(Vector3D(3, -2, -4), 0.6, Color(255, 255, 255)));
    scene->lights.push_back(Light(Vector3D(-3, 2, -4), 0.3, Color(255,255,255)));

    //CLICK HERE JUST TO TEST RAYTRACER
//    ui_renderButton->click ();
}

void Kamehameha::on_renderButton_clicked()
{
    // TODO get resolution from user input
    qDebug() << "test 1";

    Wireframer wf = Wireframer(width, height, scene);

    QFile file("out.ppm"); // TODO: file selection
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);

        watcher = new QFutureWatcher<QImage>();

        QList<QImage> images; //the qtconcurrentmap will be applied to this list

        for(int i = 0; i < width; i+= width/numberOfSamples) {
            for(int j = 0; j < height; j += height/numberOfSamples) {
                QImage img = QImage(width/numberOfSamples, height/numberOfSamples, QImage::Format_RGB32);
                img.setOffset(QPoint(i, j));
                images << img;
            }
        }

        std::function<QImage(const QImage&)> rendered = [this](const QImage &image) -> QImage
        {
            RayTracer rt = RayTracer(this->graphicsView->width (), this->graphicsView->height (), scene);
            return rt.generate(this->ui_renderProgressBar, image);
        };
        connect(watcher, SIGNAL(resultReadyAt(int)), this, SLOT(processImage(int)));
        watcher->setFuture (QtConcurrent::mapped(images, rendered));

//        //TEST GENERATE WIREFRAME IMAGE (3)
//        graphicsScene->addPixmap (QPixmap::fromImage(wf.generate (ui_renderProgressBar, 0, 0, rt.w, rt.h)));
        graphicsView->setScene(graphicsScene);
    }
}

void Kamehameha::on_cancelButton_clicked()
{
    QApplication::quit(); // placeholder

}

void Kamehameha::processImage(int index) {
    QImage img = watcher->resultAt(index);
    QGraphicsPixmapItem* item = graphicsScene->addPixmap(QPixmap::fromImage (img));
    ui_renderProgressBar->setValue(ui_renderProgressBar->value() + img.width() * img.height());
    item->setPos(img.offset ());
}

Kamehameha::~Kamehameha()
{
    delete watcher;
    delete scene;
    delete ui;
}
