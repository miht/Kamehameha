#ifndef TRACER_H
#define TRACER_H

#include <QProgressBar>
#include <QTextStream>
#include <random>

#include <QImage>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

#include "renderer.h"
#include "objparser.h"
#include "geometry.h"
#include "color.h"
#include "model.h"
#include "light.h"
#include "intersection.h"

class Tracer : public Renderer
{
public:
    int depth = 2;
    bool antiAliasing = false;

    Tracer(Scene *scene);
    QImage generate(QProgressBar *progress, QImage image) override;

    virtual Color trace(Ray ray, int depth);

private:
};

#endif // TRACER_H
