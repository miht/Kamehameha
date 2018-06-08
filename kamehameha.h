#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtConcurrent>
#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QProgressBar>
#include <QPixmap>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QColorDialog>
#include <QComboBox>
#include <QAction>
#include <QMenuBar>
#include <QButtonGroup>

#include "fbxsdk.h"
#include "fbxsdk/scene/fbxscene.h"
#include "fbxsdk/core/fbxmanager.h"
#include "fbxsdk/fileio/fbximporter.h"

#include <QFutureWatcher>

#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "raytracer.h"
#include "pathtracer.h"
#include "wireframer.h"
#include "rasterizer.h"
#include "scene.h"
#include "fbxparser.h"
#include "kdnode.h"

namespace Ui {
class MainWindow;
}

class Kamehameha : public QMainWindow
{
    Q_OBJECT

public:
    bool isRendering = false;

    Scene *scene;
    Settings *settings;

    explicit Kamehameha(QWidget *parent = 0);

    static int showMessageDialog(QString title, QString message);

    ~Kamehameha();

private slots:
    void on_renderButton_clicked();

    void on_toolButton_clicked();

    void on_rb_ortho_clicked();

    void on_rb_persp_clicked();

    void on_btn_changeColor_clicked();

    void on_btn_changeColor_bkground_clicked();

    void show_about();

    void on_rb_wireframe_clicked();
    void on_rb_raster_clicked();
    void on_lineEdit_posX_editingFinished();
    void on_lineEdit_posY_editingFinished();
    void on_lineEdit_posZ_editingFinished();
    void on_lineEdit_rotX_editingFinished();
    void on_lineEdit_rotY_editingFinished();
    void on_lineEdit_rotZ_editingFinished();

    void on_lineEdit_fov_editingFinished();

private:
    Ui::MainWindow *ui;
    QPushButton *ui_renderButton;
    QPushButton *ui_cancelButton;

    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;

    QTextBrowser *ui_logBrowser;
    QProgressBar *ui_renderProgressBar;

    QComboBox *ui_resComboBox;
    std::vector<std::pair<int, int>> resolutions;

    QLineEdit *ui_widthField;
    QLineEdit *ui_heightField;

    QButtonGroup *bg_preview;
    QButtonGroup *bg_tracing;
    QButtonGroup *bg_camera_mode;

    QSlider *ui_sampleSlider;
    QSlider *ui_subdivisionSlider;
    QSlider *ui_depthSlider;
    QCheckBox *ui_globalIlluCheckbox;

    QLabel *lbl_numVertices;
    QLabel *lbl_numFaces;

    bool addResolution(const std::pair<int, int> res);

    bool importModel(const QString path, Model &model);
    QIcon getColoredIcon(int width, int height, const QColor color);

    void updateSettings();
    void updatePreview();
};

#endif // MAINWINDOW_H
