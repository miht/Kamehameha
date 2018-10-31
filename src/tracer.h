#ifndef TRACER_H
#define TRACER_H

#include <QProgressBar>
#include <QTextStream>
#include <random>

#include <QImage>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <QFutureWatcher>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QtConcurrent>
#include <QVBoxLayout>

#include <QMenuBar>

#include "renderer.h"
#include "objparser.h"
#include "geometry.h"
#include "color.h"
#include "model.h"
#include "light.h"
#include "intersection.h"
#include "settings.h"

class Tracer : public Renderer
{
    Q_OBJECT
public:
    QMenuBar *menubar;

    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;

    QFutureWatcher<QImage> *watcher; //for parallell tracing

    Tracer();
    Tracer(QWidget *parent, Scene *scene, Settings *settings);

    void render() override;
    QImage generate(QImage image) override;

    virtual Color trace(Ray ray, int depth);

    ~Tracer();

private:

private slots:

public slots:
    void processImage(int index);
    void export_image();
};

#endif // TRACER_H
