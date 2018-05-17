#ifndef RENDERER_H
#define RENDERER_H

#include <QProgressBar>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include "scene.h"
#include "geometry.h"

class Renderer
{
public:
    enum Mode {Raytracer, Pathtracer, Rasterizer, Wireframer};
    Mode mode;
    Scene *scene;

    Renderer(Scene *scene);
    virtual QImage generate(QProgressBar *progress, QImage image);

    void setScene(Scene *scene);

protected:
    Vector3D viewportToWorld(const Vector2D vec);
    Vector2D worldToViewport(const Vector3D vec);

    float edgeFunction(const Vector3D v1, const Vector3D v2, const Vector3D c);
};

#endif // RENDERER_H
