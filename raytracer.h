#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QProgressBar>
#include <QTextStream>
#include <QImage>
#include <QGraphicsScene>
#include <QPainter>

#include <QDebug>

#include "objparser.h"
#include "vector3d.h"
#include "model3d.h"

class RayTracer
{
public:
    RayTracer();
    RayTracer(int w, int h);
    QImage trace(QProgressBar *progress, int xOffset, int yOffset, int width, int height);

    int w;
    int h;

private:
    Model3D mod;

    Vector3D screenToWorldCoordinates(const Vector3D vec);
    Vector3D worldToScreenCoordinates(const Vector3D vec);
};

#endif // RAYTRACER_H
