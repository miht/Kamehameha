#ifndef RENDERER_H
#define RENDERER_H

#include <QProgressBar>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include "scene.h"
#include "vector3d.h"

class Renderer
{
public:
    int w, h;
    Scene scene;

    Renderer(Scene &scene);
    Renderer(int width, int height, Scene &scene);
    virtual QImage generate(QProgressBar *progress, int xOffset, int yOffset, int width, int height);

    void setScene(Scene &scene);
    Scene getScene() const;

protected:
    const int DEFAULT_WIDTH = 640;
    const int DEFAULT_HEIGHT = 480;

    Vector3D screenToWorldCoordinates(const Vector3D vec);
    Vector3D worldToScreenCoordinates(const Vector3D vec);
};

#endif // RENDERER_H
