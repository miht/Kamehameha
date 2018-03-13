#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QProgressBar>
#include <QTextStream>
#include <QImage>
#include <QGraphicsScene>
#include <QPainter>

#include <QDebug>

#include "renderer.h"
#include "objparser.h"
#include "vector3d.h"
#include "color.h"
#include "model.h"
#include "light.h"
#include "ray.h"
#include "intersection.h"

class RayTracer : public Renderer
{
public:
    RayTracer(Scene &scene);
    RayTracer(int w, int h, Scene &scene);
    QImage generate(QProgressBar *progress, int xOffset, int yOffset, int width, int height) override;

private:
    Color trace(Ray ray, int depth);
    Intersection findIntersection(Ray ray);
};

#endif // RAYTRACER_H
