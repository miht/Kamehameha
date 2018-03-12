#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtConcurrent>
#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QProgressBar>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "ui_mainwindow.h"
#include "raytracer.h"
#include "wireframer.h"

namespace Ui {
class MainWindow;
}

class Kamehameha : public QMainWindow
{
    Q_OBJECT

public:
    int width, height;
    explicit Kamehameha(QWidget *parent = 0);

    ~Kamehameha();

private slots:
    void on_renderButton_clicked();
    void on_cancelButton_clicked();

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
