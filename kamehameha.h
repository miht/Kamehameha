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

#include <QFutureWatcher>

#include "ui_mainwindow.h"
#include "raytracer.h"
#include "wireframer.h"
#include "scene.h"

namespace Ui {
class MainWindow;
}

class Kamehameha : public QMainWindow
{
    Q_OBJECT

public:
    int width, height;
    int numberOfSamples = 16;
    QFutureWatcher<QImage> *watcher;
    Scene *scene;
    explicit Kamehameha(QWidget *parent = 0);

    ~Kamehameha();

private slots:
    void on_renderButton_clicked();
    void on_cancelButton_clicked();
    void processImage(int index);

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
