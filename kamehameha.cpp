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

    QString numberInputMask = "9000";
    ui_widthField = ui->lineEdit_camWidth;
    ui_widthField->setInputMask (numberInputMask);
    ui_heightField = ui->lineEdit_camHeight;
    ui_heightField->setInputMask (numberInputMask);

    //Sliders
    ui_depthSlider = ui->slider_depth;
    ui_subdivisionSlider = ui->slider_subdivision;
    ui_sampleSlider = ui->slider_samples;
    ui_globalIlluCheckbox = ui->checkbox_global_illu;

    ui_renderProgressBar->setValue(0);

    //Initialize graphic elements and configure the graphics view
    graphicsScene = new QGraphicsScene(this);
    graphicsView = ui->graphicsView;
    graphicsView->setSceneRect (0,0,graphicsView->width(), graphicsView->height ());

    scene = new Scene(0.1, Color(1,1,1));
    scene->camera.mode = Camera::perspective;

    // configure progress bar
    ui_renderProgressBar->setMinimum(0);
    ui_renderProgressBar->setMaximum(scene->camera.viewportWidth * scene->camera.viewportHeight);

    //hide the cancel button
    ui_cancelButton->setVisible(false);

    ui_widthField->setText (QString::number(scene->camera.viewportWidth));
    ui_heightField->setText (QString::number(scene->camera.viewportHeight));

    //set to wireframe rendering mode at the start
    ui->radioButton_4->click();

    graphicsView->setScene(graphicsScene);

    scene->lights.push_back(Light(Vector3D(3, 3, 1), 0.6, Color(1, 1, 1)));
    scene->lights.push_back(Light(Vector3D(-1, -3, -5), 0.3, Color(1,1,1)));

    //Initialize the watcher
    watcher = new QFutureWatcher<QImage>();
}

void Kamehameha::on_renderButton_clicked()
{
    //Enable the cancel button
    if(watcher->isRunning()) {
        if(watcher->isPaused ()) {
            ui_renderButton->setText("Pause");
            watcher->resume();
        }
        else {
            ui_renderButton->setText("Resume");
            watcher->pause();
        }
    }
    else {
        if(scene->model.objects.size () > 0) {
            qDebug() << "Model is ok";
            //Clear the progress bar
            ui_renderProgressBar->setValue(0);
            //show the cancel button
            ui_cancelButton->setVisible(true);

            //Reset the progress bar
            ui_renderProgressBar->setValue(0);
            startRender();
            ui_renderButton->setText("Pause");
        }
        else {
           showMessageDialog ("Bög", "No objects found. Did you specify a non-empty .obj file?");
        }
    }
}

void Kamehameha::on_cancelButton_clicked()
{
    stopRender();
    resetRender();
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
    ui_renderProgressBar->setValue(ui_renderProgressBar->value() + img.width() * img.height());

    img = img.scaled (scaleX * img.width (), scaleY * img.height ());

    QGraphicsPixmapItem* item = graphicsScene->addPixmap(QPixmap::fromImage (img));

    item->setPos(offsetX, offsetY);
}

void Kamehameha::startRender() {
    QList<QImage> images; //the qtconcurrentmap will be applied to this list

    switch(renderer->mode) {
    case Renderer::Pathtracer:
    {
        PathTracer* pt = dynamic_cast<PathTracer*>(renderer);
//        pt->globalIllumination =
        int subdivisions = ui->slider_subdivision->value ();
        pt->depth = ui->slider_depth->value ();
        pt->samples = ui->slider_samples->value ();
        pt->globalIllumination = ui->checkbox_global_illu->checkState () == Qt::Checked;

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
        connect(watcher, SIGNAL(finished()), this, SLOT(finishRender()));
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

        graphicsScene->addPixmap (QPixmap::fromImage(wf->generate (ui_renderProgressBar, image.scaled (graphicsView->width (), graphicsView->height ()))));
        break;
    }
    default:
    {
        break;
    }
    }
}

void Kamehameha::finishRender () {
    stopRender();
    resetRender();
}

void Kamehameha::stopRender () {
    watcher->cancel ();
    //    ui_renderProgressBar->setValue(ui_renderProgressBar->maximum ());
}

void Kamehameha::resetRender () {
    ui_cancelButton->setVisible(false);
    ui_renderButton->setText("Render");
    ui_renderProgressBar->setValue(0);
}

Kamehameha::~Kamehameha()
{
    delete watcher;
    delete renderer;
    delete scene;
    delete ui;
}

void Kamehameha::updateResolution () {
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

void Kamehameha::on_lineEdit_camWidth_editingFinished()
{
    bool ok = true;
    int w = ui_widthField->text ().toInt (&ok, 10);
    qDebug() << scene->camera.viewportWidth;
    qDebug() << scene->camera.viewportHeight;

    scene->camera.viewportWidth = w;
}

void Kamehameha::on_lineEdit_camHeight_editingFinished()
{
    bool ok = true;
    int h = ui_heightField->text ().toInt (&ok, 10);
    qDebug() << scene->camera.viewportWidth;
    qDebug() << scene->camera.viewportHeight;

    scene->camera.viewportHeight = h;
}

int Kamehameha::showMessageDialog(QString title, QString message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.exec();
}
