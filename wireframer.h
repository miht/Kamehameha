#ifndef WIREFRAMER_H
#define WIREFRAMER_H

#include <QImage>

#include "renderer.h"
#include "vector3d.h"

class Wireframer : public Renderer
{
public:
    Wireframer(Scene *scene);
    QImage generate(QProgressBar *progress, QImage image) override;
};

#endif // WIREFRAMER_H
