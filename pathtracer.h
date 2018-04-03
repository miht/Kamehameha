#ifndef PATHTRACER_H
#define PATHTRACER_H

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

class PathTracer : public Renderer
{
public:
    int subdivisions = 4;
    int samples = 16;
    int depth = 2;
    bool globalIllumination = false;

    PathTracer(Scene *scene);
    QImage generate(QProgressBar *progress, QImage image) override;

private:
    Color trace(Ray ray, int depth);
    Intersection findIntersection(Ray ray, float &t0, float &t1);
    void createCoordinateSystem(const Vector3D &N, Vector3D &Nt, Vector3D &Nb);
    Vector3D uniformSampleHemisphere(const float &r1, const float &r2);

};

#endif // PATHTRACER_H
