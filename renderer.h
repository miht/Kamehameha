#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QProgressBar>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMainWindow>

#include "geometry.h"
#include "scene.h"
#include "settings.h"

class Renderer : public QMainWindow
{
public:
    enum Mode {Raytracer, Pathtracer, Rasterizer, Wireframer};
    Mode mode;

    int viewportWidth = 512;
    int viewportHeight = 512;

    Scene *scene;
    Settings *settings;

    Renderer(QWidget *parent, Scene *scene, Settings *settings);
    virtual void render();
    virtual QImage generate(QImage image);

    void setScene(Scene *scene);

protected:
    Vector3D viewportToWorld(const Vector2D vec);
    Vector2D worldToViewport(const Vector3D vec);

    float edgeFunction(const Vector3D v1, const Vector3D v2, const Vector3D c);
};

#endif // RENDERER_H
