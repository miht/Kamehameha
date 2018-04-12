#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <QProgressBar>
#include <QTextStream>
#include <QImage>
#include <QGraphicsScene>
#include <QPainter>

#include <QDebug>

#include "tracer.h"

#include "intersection.h"

class PathTracer : public Tracer
{
public:
    int subdivisions = 4;
    int samples = 16;
    bool globalIllumination = false;

    PathTracer(Scene *scene);
    Color trace(Ray ray, int depth) override;

private:
    void createCoordinateSystem(const Vector3D &N, Vector3D &Nt, Vector3D &Nb);
    Vector3D uniformSampleHemisphere(const float &r1, const float &r2);

};

#endif // PATHTRACER_H
