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

    ui_renderProgressBar->reset();

    //Initialize graphic elements and configure the graphics view
    graphicsScene = new QGraphicsScene(this);
    graphicsView = ui->graphicsView;
    graphicsView->setSceneRect (0,0,graphicsView->width(), graphicsView->height ());

    scene = new Scene(0.5, Color(1,1,1));
    scene->camera.mode = Camera::perspective;
    scene->camera.imageWidth = graphicsView->width ();
    scene->camera.imageHeight = graphicsView->height();

    // configure progress bar
    ui_renderProgressBar->setMinimum(0);
    ui_renderProgressBar->setMaximum(scene->camera.imageWidth * scene->camera.imageHeight);

    //hide the cancel button
    ui_cancelButton->setVisible(false);

    ui_widthField->setText (QString::number(scene->camera.imageWidth));
    ui_heightField->setText (QString::number(scene->camera.imageHeight));

    //set to path rendering mode at the start
    ui->radioButton->click();

    graphicsView->setScene(graphicsScene);

//    scene->lights.push_back(Light(Vector3D(0,0,0), 1, Color(1, 1, 1)));

//    scene->lights.push_back(Light(Vector3D(1.5, 1.5, -0.5), 0.3, Color(1, 1, 1)));
//    scene->lights.push_back(Light(Vector3D(-0.5, -0.5, 2.5), 0.6, Color(1,1,1)));

    //Initialize the watcher
    watcher = new QFutureWatcher<QImage>();
}

void Kamehameha::on_renderButton_clicked()
{
    //Enable the cancel button
    if(state != rendering) {
        if(state == paused) {
            ui_renderButton->setText("Pause");
            watcher->resume();
            state = rendering;
        }
        else {
            if(scene->model.root != NULL) {
                //show the cancel button
                ui_cancelButton->setVisible(true);

                //Reset the progress bar
                ui_renderProgressBar->setValue(0);
                startRender();
                ui_renderButton->setText("Pause");
                state = rendering;
            }
            else {
               showMessageDialog ("Bög", "No objects found. Did you specify a non-empty .obj file?");
            }
        }
    }
    else {
        ui_renderButton->setText("Resume");
        watcher->pause();
        state = paused;
    }
}

void Kamehameha::on_cancelButton_clicked()
{
    cancelRender ();
    resetRender();
}

/**
 * @brief Kamehameha::processImage scales the image and inserts it with the proper
 * offset within the graphicsview
 * @param index
 */
void Kamehameha::processImage(int index) {
    QImage img = watcher->resultAt(index);
    float scaleX = graphicsView->width () / scene->camera.imageWidth;
    float scaleY = graphicsView->height () / scene->camera.imageHeight;

    float offsetX = img.offset().x() * scaleX;
    float offsetY = img.offset().y() * scaleY;
    ui_renderProgressBar->setValue(ui_renderProgressBar->value() + img.width() * img.height());

    img = img.scaled (scaleX * img.width (), scaleY * img.height ());
    QGraphicsPixmapItem* item = graphicsScene->addPixmap(QPixmap::fromImage (img));

    item->setPos(offsetX, offsetY);

    if(watcher->progressValue () >= watcher->progressMaximum ()) {
        resetRender ();
    }
}

void Kamehameha::startRender() {
    QList<QImage> images; //the qtconcurrentmap will be applied to this list

    applySettings();

    switch(renderer->mode) {
    case Renderer::Pathtracer:
    {
        PathTracer* pt = dynamic_cast<PathTracer*>(renderer);
        int subdivisions = ui->slider_subdivision->value ();
        pt->depth = ui->slider_depth->value ();
        pt->samples = ui->slider_samples->value ();
        pt->globalIllumination = ui->checkbox_global_illu->checkState () == Qt::Checked;
        pt->antiAliasing = ui->checkbox_anti_alias->checkState () == Qt::Checked;

        for(int i = 0; i < scene->camera.imageWidth; i+= scene->camera.imageWidth/subdivisions) {
            for(int j = 0; j < scene->camera.imageHeight; j += scene->camera.imageHeight/subdivisions) {
                QImage img = QImage(scene->camera.imageWidth/subdivisions, scene->camera.imageHeight/subdivisions, QImage::Format_RGB32);
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
        int subdivisions = ui->slider_subdivision->value ();
        rt->depth = ui->slider_depth->value ();
        rt->antiAliasing = ui->checkbox_anti_alias->checkState () == Qt::Checked;

        for(int i = 0; i < scene->camera.imageWidth; i+= scene->camera.imageWidth/subdivisions) {
            for(int j = 0; j < scene->camera.imageHeight; j += scene->camera.imageHeight/subdivisions) {
                QImage img = QImage(scene->camera.imageWidth/subdivisions, scene->camera.imageHeight/subdivisions, QImage::Format_RGB32);
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
        Rasterizer* r = dynamic_cast<Rasterizer*>(renderer);
        QImage image(scene->camera.imageWidth, scene->camera.imageHeight, QImage::Format_ARGB32);

        float scaleX = graphicsView->width () / scene->camera.imageWidth;
        float scaleY = graphicsView->height () / scene->camera.imageHeight;

        graphicsScene->addPixmap (QPixmap::fromImage(r->generate (ui_renderProgressBar, image).scaled (scaleX * image.width (), scaleY * image.height ())));

        break;
    }
    case Renderer::Wireframer:
    {
        Wireframer* wf = dynamic_cast<Wireframer*>(renderer);
        QImage image(scene->camera.imageWidth, scene->camera.imageHeight, QImage::Format_ARGB32);
        float scaleX = graphicsView->width () / scene->camera.imageWidth;
        float scaleY = graphicsView->height () / scene->camera.imageHeight;

        graphicsScene->addPixmap (QPixmap::fromImage(wf->generate (ui_renderProgressBar, image).scaled (scaleX * image.width (), scaleY * image.height ())));

        break;
    }
    default:
    {
        break;
    }
    }
}

void Kamehameha::cancelRender () {
    watcher->cancel ();
}

void Kamehameha::pauseRender () {
    qDebug() << "Pausing";
    state = paused;
}

void Kamehameha::resetRender () {
    state = cancelled;
    ui_renderProgressBar->reset ();
    ui_cancelButton->setVisible(false);
    ui_renderButton->setText("Render");
}

Kamehameha::~Kamehameha()
{
    delete watcher;
    delete renderer;
    delete scene->model.root;
    delete scene;
    delete ui;
}

void Kamehameha::updateResolution () {
}

void Kamehameha::on_toolButton_clicked()
{
    QString path = "/Users/leifthysellsundqvist/Documents/Kamehameha/res/";
    QString file;
    QString dir;

    file = QFileDialog::getOpenFileName(this,
                                        tr("Load Scene"), path, tr("Scenes (*.fbx)"));

    if(file == "")
        return;

    QFileInfo info(file);
    file = info.absoluteFilePath ();
    dir = info.absolutePath () + "/";

//    scene->model = ObjParser::parse(dir, file);
//    scene->model = importModel (file);
    bool imported = importModel (file, scene->model);

//    ui->btn_lightColor->setIcon (getColoredIcon (100, 100, Color((scene->ambient_color.asVector3D ()*scene->ambient_intensity)).asQColor ());
//    ui->btn_lightColor_bkground->setIcon (getColoredIcon (100, 100, scene->ambient_color*scene->ambient_intensity));

//    ui->lbl_edges->setText ("" + 0);
//    ui->lbl_faces->setText("" + scene->model.root->faces.size());
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

void Kamehameha::on_radioButton_2_clicked()
{
    renderer = new Rasterizer(scene);
}

void Kamehameha::on_lineEdit_camWidth_editingFinished()
{
//    bool ok = true;
//    int w = ui_widthField->text ().toInt (&ok, 10);
//    scene->camera.imageWidth = w;
}

void Kamehameha::on_lineEdit_camHeight_editingFinished()
{
//    bool ok = true;
//    int h = ui_heightField->text ().toInt (&ok, 10);
//   scene->camera.imageHeight = h;
}

int Kamehameha::showMessageDialog(QString title, QString message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    return msgBox.exec();
}

void Kamehameha::on_rb_ortho_clicked()
{
    scene->camera.mode = Camera::ortographic;
}

void Kamehameha::on_rb_persp_clicked()
{
    scene->camera.mode = Camera::perspective;
}

void Kamehameha::applySettings() {
    //Attempt to parse numbers
    bool ok = true;

    int w = ui_widthField->text ().toInt (&ok, 10);
    int h = ui_heightField->text ().toInt (&ok, 10);
    scene->camera.imageWidth = w;
    scene->camera.imageHeight = h;
    scene->camera.angleOfView = ui->lineEdit_fov->text ().toInt(&ok, 10);
    bool ambient_intensity = ((float)ui->slider_ambientIntensity->value() / (float)ui->slider_ambientIntensity->maximum ());

    float camTranslateX = ui->lineEdit_posX->text().toFloat (&ok);
    float camTranslateY = ui->lineEdit_posY->text().toFloat (&ok);
    float camTranslateZ = ui->lineEdit_posZ->text().toFloat (&ok);

    float camRotateX = ui->lineEdit_rotX->text().toFloat (&ok);
    float camRotateY = ui->lineEdit_rotY->text().toFloat (&ok);
    float camRotateZ = ui->lineEdit_rotZ->text().toFloat (&ok);

    Matrix4x4 translation = Matrix4x4::translation (Vector3D(camTranslateX,
                                                             camTranslateY,
                                                             camTranslateZ));

    Matrix4x4 rotation = Matrix4x4::rotation (Vector3D(camRotateX * M_PI/180,
                                                       camRotateY * M_PI/180,
                                                       camRotateZ * M_PI/180));

    scene->camera.camToWorld = translation * rotation;
}

void Kamehameha::on_btn_changeColor_clicked()
{
    QColor color = QColorDialog::getColor (QColor(255,255,255), this, "Ambient color");

    ui->btn_lightColor->setIcon(getColoredIcon(100, 100, color));
}

void Kamehameha::on_btn_changeColor_bkground_clicked()
{
    QColor color = QColorDialog::getColor (QColor(255,255,255), this, "Background color");
    ui->btn_lightColor_bkground->setIcon(getColoredIcon(100, 100, color));

//    // set black background
//    pal.setColor(QPalette::Background, color);
//    ui->btn_lightColor_bkground->setAutoFillBackground(true);
//    ui->btn_lightColor_bkground->setPalette (pal);
//    ui->btn_lightColor_bkground->update ();

}

bool Kamehameha::importModel(const QString path, Model &model) {
    qDebug() << "Attempting to import FBX\n";

    // Create the FBX SDK manager
    FbxManager* lSdkManager = FbxManager::Create();

    // Create an IOSettings object.
    FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
    lSdkManager->SetIOSettings(ios);

    // ... Configure the FbxIOSettings object ...

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Declare the path and filename of the file containing the scene.
    // In this case, we are assuming the file is in the same directory as the executable.
    const char* lFilename = path.toLatin1();

    qDebug() << lFilename;

    // Initialize the importer.
    bool lImportStatus = lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());

    if(lImportStatus == false) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        showMessageDialog ("Error", QString(lImporter->GetStatus().GetErrorString()));
        return false;
    }

    // Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager,"Scene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();

    //triangulate scene
    FbxGeometryConverter conv(lSdkManager);
    conv.Triangulate (lScene, true);

    FbxNode* root = lScene->GetRootNode ();
    std::vector<Face*> faces;
    std::vector<Light> lights;
    std::vector<Material> materials;

    FbxParser::process(root, faces, lights, scene->model.materials);

//    FbxParser::processMaterials (root, materials);

    scene->model.root = new KDNode();
    scene->model.root = scene->model.root->build(faces, 0);
    scene->lights = lights;


//    FbxMesh*mesh = root->GetMesh ();


//    model.numberOfFaces = mesh->GetPolygonCount ();
//    model.numberOfVertices = mesh->GetPolygonVertexCount ();

//    qDebug() << "Test 2";

//    //Create the triangles. TODO: transition entirely to using FBX polygons instead of our native geometry?
//    std::vector<Face*> faces;

//    qDebug() << "Test 3";



    return true;
}

QIcon Kamehameha::getColoredIcon(int width, int height, const QColor color) {
    QPixmap pixmap(width, height);
    pixmap.fill(color);
    QIcon icon(pixmap);
    return icon;
}

