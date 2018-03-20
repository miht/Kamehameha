#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QImage>

#include "renderer.h"
#include "vector3d.h"

class Rasterizer : public Renderer
{
public:
    Rasterizer(Scene *scene);
    Rasterizer(int w, int h, Scene *scene);
    QImage generate(QProgressBar *progress, int xOffset, int yOffset, int width, int height) override;
};

#endif // RASTERIZER_H
