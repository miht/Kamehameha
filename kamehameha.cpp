#include "kamehameha.h"
#include "ui_mainwindow.h"
#include "raytracer.h"

#include <QFile>
#include <QTextStream>

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
}

void Kamehameha::on_renderButton_clicked()
{
    // TODO get resolution from user input
    RayTracer rt = RayTracer();

    // configure progress bar
    ui_renderProgressBar->setMinimum(0);
    ui_renderProgressBar->setMaximum(rt.w * rt.h);

    QFile file("out.ppm"); // TODO: file selection
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        rt.trace(out, ui_renderProgressBar);
    }

    file.close();
}

void Kamehameha::on_cancelButton_clicked()
{
    QApplication::quit(); // placeholder

}

Kamehameha::~Kamehameha()
{
    delete ui;
}
