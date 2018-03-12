#ifndef WIREFRAMER_H
#define WIREFRAMER_H

#include <QImage>

#include "renderer.h"
#include "vector3d.h"

class Wireframer : public Renderer
{
public:
    Wireframer(Scene &scene);
    Wireframer(int w, int h, Scene &scene);
    QImage generate(int width, int height); //the total wisth and height of the image. no parallel computations here
};

#endif // WIREFRAMER_H
