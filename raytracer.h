#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QProgressBar>
#include <QTextStream>
#include <QPixmap>
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
    void trace(QTextStream  &out, QProgressBar *progress, QPixmap &pixmap);
    int w;
    int h;

private:
    Vector3D screenToWorldCoordinates(const Vector3D vec);
    Vector3D worldToScreenCoordinates(const Vector3D vec);
};

#endif // RAYTRACER_H
