#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "tracer.h"

class RayTracer : public Tracer
{
public:
    int subdivisions = 4;

    RayTracer(QWidget *parent, Scene *scene, Settings *settings);

    Color trace(Ray ray, int depth) override;

private:
};

#endif // RAYTRACER_H
