#include "kamehameha.h"

Kamehameha::Kamehameha(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // get widgets
    ui_renderButton = findChild<QPushButton*>("renderButton");
    ui_logBrowser = findChild<QTextBrowser*>("logBrowser");
    ui_renderProgressBar = findChild<QProgressBar*>("renderProgressBar");

    //Sliders
    ui_depthSlider = ui->slider_depth;
    ui_subdivisionSlider = ui->slider_subdivision;
    ui_sampleSlider = ui->slider_samples;
    ui_globalIlluCheckbox = ui->checkbox_global_illu;

    ui_renderProgressBar->reset();

    graphicsView = ui->graphicsView;
    graphicsView->setSceneRect (0,0,graphicsView->width (),graphicsView->height ());

    menuBar ()->setNativeMenuBar (false);
    QMenu *file = menuBar ()->addMenu (tr("File"));
    QMenu *help = menuBar ()->addMenu(tr("Help"));
    QAction *about = help->addAction(tr("About Helios"), this, SLOT(show_about()));
    about->setStatusTip(tr("Show information about Helios"));

    //Configure combobox with available resolutions
    ui_resComboBox = ui->comboBox_resolution;

    addResolution (std::make_pair(400, 300));
    addResolution (std::make_pair(800, 600));
    addResolution (std::make_pair(1024, 768));
    addResolution (std::make_pair(1366, 768));
    addResolution (std::make_pair(1920, 1080));

    //button groups
    bg_preview = new QButtonGroup(this);
    bg_tracing = new QButtonGroup(this);
    bg_camera_mode = new QButtonGroup(this);

    //TODO assign these static global ID's instead of having hardcoded ones...
    ui->rb_raster->setChecked(true);
    bg_tracing->addButton (ui->rb_ray, 0);
    bg_tracing->addButton (ui->rb_path, 1);

    bg_preview->addButton (ui->rb_raster, 0);
    bg_preview->addButton (ui->rb_wireframe, 1);

    bg_camera_mode->addButton (ui->rb_ortho, 0);
    bg_camera_mode->addButton (ui->rb_persp, 1);

    // configure progress bar
    ui_renderProgressBar->setMinimum(0);
    //    ui_renderProgressBar->setMaximum(scene->camera.viewportWidth * scene->camera.viewportHeight);

    //Initialize graphic elements and configure the graphics view
    graphicsScene = new QGraphicsScene(this);
    graphicsView->setScene(graphicsScene);

    //Settings
    settings = new Settings();
    //    updateSettings();
}

void Kamehameha::on_renderButton_clicked()
{
    if(scene->isLoaded) {

        //Reset the progress bar
        ui_renderProgressBar->setValue(0);
        updateSettings();

        Tracer *tracer;

        switch(bg_tracing->checkedId ()) {
        case 0: {
            //Ray
            //            tracer = RayTracer(&scene, settings);
            qDebug() << "Ray tracing...";
            tracer = new RayTracer(nullptr, scene, settings);
            break;
        }
        case 1: {
            //Path
            qDebug() << "Path tracing...";
            tracer = new PathTracer(nullptr, scene, settings);
            break;
        }
        default: {
            break;
        }
        }

        tracer->viewportWidth = settings->width;
        tracer->viewportHeight = settings->height;

        tracer->render ();
        tracer->show ();
    }
    else {
        showMessageDialog (tr("Error"), tr("No objects found. Please specify a non-empty model."));
    }

}

Kamehameha::~Kamehameha()
{
    delete scene;
    delete settings;

    delete ui;
}

void Kamehameha::updatePreview() {
    Renderer *preview;

    qDebug() << bg_preview->checkedId ();
    switch(bg_preview->checkedId ()) {
    case 0:
    {
        //Raster
        qDebug() << "Raster";

        preview = new Rasterizer(this, scene, settings);
        break;
    }
    case 1:
    {
        //Wireframe
        qDebug() << "Wireframe";

        preview = new Wireframer(this, scene, settings);
        break;
    }
    default:
    {
        qDebug() << "How?" ;
        break;
    }
    }

    preview->viewportWidth = graphicsView->width ();
    preview->viewportHeight = graphicsView->height ();

    QImage image(graphicsView->width (), graphicsView->height(), QImage::Format_ARGB32);
    qDebug() << graphicsView->width ();

    QPixmap pix = QPixmap::fromImage(preview->generate (image));

    graphicsScene->addPixmap (pix);

    delete preview;
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

    bool imported = importModel (file, scene->model);

    ui->lineEdit_path->setText (file);
    ui->lbl_faces->setText (QString::number(scene->model.metadata.numFaces));
    ui->lbl_vertices->setText(QString::number(scene->model.metadata.numVertices));
    ui->lbl_lights->setText(QString::number(scene->model.metadata.numLights));
    ui->lbl_materials->setText(QString::number(scene->model.metadata.numMaterials));

    //    ui->btn_lightColor->setIcon (getColoredIcon (100, 100, Color((scene->ambient_color.asVector3D ()*scene->ambient_intensity)).asQColor ());
    //    ui->btn_lightColor_bkground->setIcon (getColoredIcon (100, 100, scene->ambient_color*scene->ambient_intensity));


}

void Kamehameha::show_about () {
    //    showMessageDialog (tr("About Helios"), tr("Helios (2018) is a graphics engine developed by KTH students Anders Eriksson and Leif T. Sundkvist."));
    AboutDialog aboutDialog(this);
    aboutDialog.exec ();
}

int Kamehameha::showMessageDialog(QString title, QString message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    return msgBox.exec();
}

void Kamehameha::on_rb_ortho_clicked()
{
    updateSettings ();
    updatePreview ();}

void Kamehameha::on_rb_persp_clicked()
{
    updateSettings ();
    updatePreview ();}

void Kamehameha::updateSettings() {
    //Attempt to parse numbers
    qDebug() << "T1";

    bool ok = true;

    //Settings
    std::pair<int, int> resolution = resolutions[ui_resComboBox->currentIndex ()];
    settings->width = resolution.first;
    settings->height = resolution.second;
    settings->subdivisions = ui->slider_subdivision->value ();

    settings->depth = ui->slider_depth->value ();
    settings->samples = ui->slider_samples->value ();

    settings->globalIllumination = ui->checkbox_global_illu->checkState () == Qt::Checked;
    settings->antiAliasing = ui->checkbox_anti_alias->checkState () == Qt::Checked;

    //Scene
    qDebug() << "T1.5";

    //Camera
    qDebug() << scene->camera.angleOfView;
    scene->camera.viewportWidth = settings->width; //These two redundant? perhaps TOOD remove?

    scene->camera.viewportHeight = settings->height;
    scene->camera.angleOfView = ui->lineEdit_fov->text ().toInt(&ok, 10);


    switch(bg_camera_mode->checkedId ()) {
    case 0: {
        //Orthographic
        scene->camera.mode = Camera::ortographic;
        break;
    }
    case 1: {
        //Perspective
        scene->camera.mode = Camera::perspective;
        break;
    }
    default: {
        break;
    }
    }

    bool ambient_intensity = ((float)ui->slider_ambientIntensity->value() / (float)ui->slider_ambientIntensity->maximum ());

    float camTranslateX = ui->lineEdit_posX->text().toFloat (&ok);
    float camTranslateY = ui->lineEdit_posY->text().toFloat (&ok);
    float camTranslateZ = ui->lineEdit_posZ->text().toFloat (&ok);

    qDebug() << "Updating rotation!";
    float camRotateX = ui->lineEdit_rotX->text().toFloat (&ok);
    float camRotateY = ui->lineEdit_rotY->text().toFloat (&ok);
    float camRotateZ = ui->lineEdit_rotZ->text().toFloat (&ok);
    qDebug() << "Updated rotation!";

    Matrix4x4 translation = Matrix4x4::translation (Vector3D(camTranslateX,
                                                             camTranslateY,
                                                             camTranslateZ));

    Matrix4x4 rotation = Matrix4x4::rotation (Vector3D(camRotateX * M_PI/180,
                                                       camRotateY * M_PI/180,
                                                       camRotateZ * M_PI/180));

    scene->camera.world = translation * rotation;

    qDebug() << "T2";
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

bool Kamehameha::addResolution(const std::pair<int, int> res) {
    char res_buffer[15];
    sprintf(res_buffer, "%d, %d", res.first, res.second);
    int index = resolutions.size ();
    ui_resComboBox->insertItem(index, res_buffer);
    resolutions.push_back (res);

    return true;
}

bool Kamehameha::importModel(const QString path, Model &model) {
    qDebug() << "Attempting to import FBX.";

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
//    qDebug() << path;
    char *lFilename = new char[100]();
    strcpy(lFilename, path.toUtf8 ());

    qDebug() << lFilename;

    // Initialize the importer.
    bool lImportStatus = lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());

    delete lFilename;

    if(lImportStatus == false) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        showMessageDialog ("Error", QString(lImporter->GetStatus().GetErrorString()));
        return false;
    }

    // Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager,"Scene");
    FbxAxisSystem directXAxisSys(FbxAxisSystem::EUpVector::eZAxis,
                                 FbxAxisSystem::EFrontVector::eParityEven,
                                 FbxAxisSystem::eRightHanded);
    directXAxisSys.ConvertScene(lScene);

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();

    qDebug() << "FBX file loaded.";

    FbxGeometryConverter conv(lSdkManager);
    conv.Triangulate (lScene, true);

    //Initialize scene

    scene = new Scene(0.5, Color(1,1,1));

    FbxNode* fbxRoot = lScene->GetRootNode ();
    scene->model.root = new KDNode();

    FbxParser::process(fbxRoot, scene);

    qDebug() << scene->model.root->faces.size();

    scene->model.root = KDNode::build(scene->model.root->faces, 0);

    updateSettings();
    updatePreview();

    qDebug() << "FBX scene successfully parsed.";

    return true;
}

QIcon Kamehameha::getColoredIcon(int width, int height, const QColor color) {
    QPixmap pixmap(width, height);
    pixmap.fill(color);
    QIcon icon(pixmap);
    return icon;
}

void Kamehameha::on_rb_wireframe_clicked()
{
    updateSettings();
    updatePreview ();
}

void Kamehameha::on_rb_raster_clicked()
{
    updateSettings();
    updatePreview();
}

void Kamehameha::on_lineEdit_posX_editingFinished ()
{
    updateSettings ();
    updatePreview ();
}

void Kamehameha::on_lineEdit_posY_editingFinished()
{
    updateSettings ();
    updatePreview ();
}

void Kamehameha::on_lineEdit_posZ_editingFinished()
{
    updateSettings ();
    updatePreview ();
}

void Kamehameha::on_lineEdit_rotX_editingFinished()
{
    updateSettings ();
    updatePreview ();
}

void Kamehameha::on_lineEdit_rotY_editingFinished()
{
    updateSettings ();
    updatePreview ();
}

void Kamehameha::on_lineEdit_rotZ_editingFinished()
{
    updateSettings ();
    updatePreview ();
}

void Kamehameha::on_lineEdit_fov_editingFinished()
{
    updateSettings ();
    updatePreview ();
}
