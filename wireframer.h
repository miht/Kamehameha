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
    QImage generate(QProgressBar *progress, int xOffset, int yOffset, int width, int height) override;
};

#endif // WIREFRAMER_H
