#include "tracer.h"

Tracer::Tracer()
    :   Renderer(nullptr, nullptr, nullptr)
{
}

Tracer::Tracer(QWidget *parent, Scene *scene, Settings *settings)
    :   Renderer(parent, scene, settings)
{
    setWindowTitle("Render output");

    int width = settings->width;
    int height = settings->height;
    setFixedSize (width, height);

    //Configure menubar
    menuBar()->setNativeMenuBar (false);
    QMenu *file = menuBar()->addMenu (tr("Image"));
    QAction *exp = file->addAction(tr("Export"), this, SLOT(export_image()));
    exp->setStatusTip(tr("Export rendered image to file"));

    //Configure layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    //Configure the output windoww
    graphicsView= new QGraphicsView();

    graphicsView->setFixedSize (width, height);
    graphicsView->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    graphicsView->setSceneRect (0,0, width, height);

    //Initialize graphic elements and configure the graphics view
    graphicsScene = new QGraphicsScene();
    graphicsView->setScene(graphicsScene);

//    layout->setMenuBar(menubar);
    mainLayout->addWidget(graphicsView);
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout (mainLayout);
    setCentralWidget (graphicsView);

    //Configure the watcher
    watcher = new QFutureWatcher<QImage>();
}

void Tracer::render() {
    QList<QImage> images; //the qtconcurrentmap will be applied to this list

    int width = settings->width;
    int height = settings->height;
    int subdivisions = settings->subdivisions;

    for(int i = 0; i < width; i+= width/subdivisions) {
        for(int j = 0; j < height; j += height/subdivisions) {
            QImage img = QImage(width/subdivisions, height/subdivisions, QImage::Format_RGB32);
            img.setOffset(QPoint(i, j));
            images << img;
        }
    }

    std::function<QImage(const QImage&)> rendered = [this](const QImage &image) -> QImage
    {
        return generate(image);
    };

    connect(watcher, SIGNAL(resultReadyAt(int)), this, SLOT(processImage(int)));
    watcher->setFuture (QtConcurrent::mapped(images, rendered));
}

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-1, 1);


QImage Tracer::generate(QImage image)
{
    //Calculate screen coordinates here
    float width = (float) scene->camera.viewportWidth;
    float height = (float) scene->camera.viewportHeight;
    float imageAspectRatio = width / height;
    float fov = scene->camera.angleOfView;
    float scale = tanf((fov * 0.5 * M_PI)/180.0);
    float focalLength = scene->camera.focalLength;

    Matrix4x4 camToWorld = scene->camera.world;
    Vector3D rayOrigin = camToWorld*Vector3D(0,0,0);

    // Iterate over all pixels in image
    for (int x = 0; x < image.width (); x++) {
        for (int y = 0; y < image.height (); y++) {
            Color col;

            Vector3D p = viewportToWorld (Vector2D(x + image.offset ().x (), y + image.offset ().y()));
            Vector3D dir;

            Vector3D camForward(camToWorld(2, 0), camToWorld(2, 1), camToWorld(2, 2));
            camForward = camForward.normalized ();


            if(scene->camera.mode == Camera::perspective) {
                dir = (p - rayOrigin);
            }
            else {
                rayOrigin = (camToWorld * p);
                dir = camForward;
//                dir = camToWorld*Vector3D(0,1,0); //forward
            }
//            qDebug() << "origin " << rayOrigin;
//            qDebug() << "cam forward " << camForward;

            dir = dir.normalized ();
            Ray ray;
            ray = Ray(rayOrigin, dir);

            col = trace(ray, settings->depth);
            image.setPixelColor(x, y, Color(col).asQColor ());
        }
    }
    return image;
}

Color Tracer::trace(Ray ray, int depth) {
    return Color(0,0,0);
}

/**
 * @brief Kamehameha::processImage scales the image and inserts it with the proper
 * offset within the graphicsview
 * @param index
 */
void Tracer::processImage(int index) {
    QImage img = watcher->resultAt(index);
    float scaleX = graphicsView->width () / scene->camera.viewportWidth;
    float scaleY = graphicsView->height () / scene->camera.viewportHeight;

    float offsetX = img.offset().x() * scaleX;
    float offsetY = img.offset().y() * scaleY;
//    ui_renderProgressBar->setValue(ui_renderProgressBar->value() + img.width() * img.height());

    img = img.scaled (scaleX * img.width (), scaleY * img.height ());
    QGraphicsPixmapItem* item = graphicsScene->addPixmap(QPixmap::fromImage (img));

    item->setPos(offsetX, offsetY);
}

void Tracer::export_image () {
    QString path = "/Users/leifthysellsundqvist/Documents/Kamehameha/res/";
    QString file;

    file = QFileDialog::getSaveFileName (this, tr("Save image"), path, tr("Images (*.png *.jpg)"));

    bool saved;

    QImage output(512, 512,  QImage::Format_RGB32);
    QPainter painter(&output);
    graphicsView->scene ()->render(&painter);
    saved = output.save(file, "png");

}

Tracer::~Tracer()
{
    delete watcher;
}

