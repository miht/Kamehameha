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
    graphicsView->setSceneRect (0,0,graphicsView->width(), graphicsView->height ());

    // configure progress bar
    ui_renderProgressBar->setMinimum(0);
    ui_renderProgressBar->setMaximum(scene->camera.viewportWidth * scene->camera.viewportHeight);

    scene = new Scene(0.1, Color(1,1,1));
    scene->camera.mode = Camera::perspective;

    graphicsView->setScene(graphicsScene);

//    scene->lights.push_back(Light(Vector3D(3, 3, 1), 0.6, Color(255, 255, 255)));
    scene->lights.push_back(Light(Vector3D(-1, -3, -5), 0.3, Color(255,255,255)));

    //    for(QString m : scene->model.materials.keys ()) {
    //        Material mat = scene->model.materials.value(m);
    //        qDebug() << m << ": " << mat;
    //    }

    //CLICK HERE JUST TO TEST RAYTRACER
    //    ui_renderButton->click ();
}

void Kamehameha::on_renderButton_clicked()
{
    watcher = new QFutureWatcher<QImage>();
    QList<QImage> images; //the qtconcurrentmap will be applied to this list

    switch(renderer->mode) {
    case Renderer::Pathtracer:
    {
        PathTracer* pt = dynamic_cast<PathTracer*>(renderer);
        int subdivisions = pt->subdivisions;

        for(int i = 0; i < scene->camera.viewportWidth; i+= scene->camera.viewportWidth/subdivisions) {
            for(int j = 0; j < scene->camera.viewportHeight; j += scene->camera.viewportHeight/subdivisions) {
                QImage img = QImage(scene->camera.viewportWidth/subdivisions, scene->camera.viewportHeight/subdivisions, QImage::Format_RGB32);
                img.setOffset(QPoint(i, j));
                images << img;
            }
        }
        std::function<QImage(const QImage&)> rendered = [this,pt](const QImage &image) -> QImage
        {
            return pt->generate(this->ui_renderProgressBar, image);
        };

        connect(watcher, SIGNAL(resultReadyAt(int)), this, SLOT(processImage(int)));
        watcher->setFuture (QtConcurrent::mapped(images, rendered));
        break;
    }
    case Renderer::Raytracer:
    {
        RayTracer* rt = dynamic_cast<RayTracer*>(renderer);
        int subdivisions = rt->subdivisions;

        for(int i = 0; i < scene->camera.viewportWidth; i+= scene->camera.viewportWidth/subdivisions) {
            for(int j = 0; j < scene->camera.viewportHeight; j += scene->camera.viewportHeight/subdivisions) {
                QImage img = QImage(scene->camera.viewportWidth/subdivisions, scene->camera.viewportHeight/subdivisions, QImage::Format_RGB32);
                img.setOffset(QPoint(i, j));
                images << img;
            }
        }
        std::function<QImage(const QImage&)> rendered = [this,rt](const QImage &image) -> QImage
        {
            return rt->generate(this->ui_renderProgressBar, image);
        };

        connect(watcher, SIGNAL(resultReadyAt(int)), this, SLOT(processImage(int)));
        watcher->setFuture (QtConcurrent::mapped(images, rendered));
        break;
    }
    case Renderer::Rasterizer:
    {
        break;
    }
    case Renderer::Wireframer:
    {
        Wireframer* wf = dynamic_cast<Wireframer*>(renderer);
        QImage image(scene->camera.viewportWidth, scene->camera.viewportHeight, QImage::Format_ARGB32);
        graphicsScene->addPixmap (QPixmap::fromImage(wf->generate (ui_renderProgressBar, image)));
        break;
    }
    default:
    {
        break;
    }
    }
}

void Kamehameha::on_cancelButton_clicked()
{
    QApplication::quit(); // placeholder

}

/**
 * @brief Kamehameha::processImage scales the image and inserts it with the proper
 * offset within the graphicsview
 * @param index
 */
void Kamehameha::processImage(int index) {
    QImage img = watcher->resultAt(index);
    float scaleX = graphicsView->width () / scene->camera.viewportWidth;
    float scaleY = graphicsView->height () / scene->camera.viewportHeight;

    float offsetX = img.offset().x() * scaleX;
    float offsetY = img.offset().y() * scaleY;

    img = img.scaled (scaleX * img.width (), scaleY * img.height ());

    QGraphicsPixmapItem* item = graphicsScene->addPixmap(QPixmap::fromImage (img));

    item->setPos(offsetX, offsetY);

    ui_renderProgressBar->setValue(ui_renderProgressBar->value() + img.width() * img.height());
}

Kamehameha::~Kamehameha()
{
    delete watcher;
    delete renderer;
    delete scene;
    delete ui;
}

void Kamehameha::on_toolButton_clicked()
{
    QString path = "/Users/leifthysellsundqvist/Library/Mobile Documents/com~apple~CloudDocs/Skolan/Y4/VisGraf18/";
    QString file;
    QString dir;

    file = QFileDialog::getOpenFileName(this,
                                        tr("Load Model"), path, tr("Model Files (*.obj)"));

    if(file == "")
        return;

    QFileInfo info(file);
    file = info.absoluteFilePath ();
    dir = info.absolutePath () + "/";

    scene->model = ObjParser::parse(dir, file);

}

//Path
void Kamehameha::on_radioButton_clicked()
{
    renderer = new PathTracer(scene);
}

//RayTracer
void Kamehameha::on_radioButton_3_clicked()
{
    renderer = new RayTracer(scene);
}


//Wireframe
void Kamehameha::on_radioButton_4_clicked()
{
    renderer = new Wireframer(scene);
}
