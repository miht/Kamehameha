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

#include <QFutureWatcher>

#include "ui_mainwindow.h"
#include "raytracer.h"
#include "pathtracer.h"
#include "wireframer.h"
#include "scene.h"

namespace Ui {
class MainWindow;
}

class Kamehameha : public QMainWindow
{
    Q_OBJECT

public:
    Renderer *renderer;
    QFutureWatcher<QImage> *watcher; //for parallell tracing
    Scene *scene;
    explicit Kamehameha(QWidget *parent = 0);

    ~Kamehameha();

private slots:
    void on_renderButton_clicked();
    void on_cancelButton_clicked();
    void processImage(int index);

    void on_toolButton_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_clicked();

    void on_radioButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QPushButton *ui_renderButton;
    QPushButton *ui_cancelButton;
    QTextBrowser *ui_logBrowser;
    QProgressBar *ui_renderProgressBar;

    QGraphicsScene *graphicsScene;
    QGraphicsView *graphicsView;
};

#endif // MAINWINDOW_H
