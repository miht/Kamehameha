#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QImage>

#include "renderer.h"
#include "vector3d.h"

class Rasterizer : public Renderer
{
public:
    Rasterizer(QWidget *parent, Scene *scene, Settings *settings);
    Rasterizer(int w, int h, Scene *scene);
    QImage generate(QImage image) override;

private:

};

#endif // RASTERIZER_H
