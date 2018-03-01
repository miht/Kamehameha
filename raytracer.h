#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QProgressBar>
#include <QTextStream>

class RayTracer
{
public:
    RayTracer();
    RayTracer(int w, int h);
    void trace(QTextStream  &out, QProgressBar *progress);
    int w;
    int h;
};

#endif // RAYTRACER_H
