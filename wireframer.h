#ifndef WIREFRAMER_H
#define WIREFRAMER_H

#include <QImage>

#include "renderer.h"
#include "vector3d.h"

class Wireframer : public Renderer
{
public:
    Wireframer(QWidget *parent, Scene *scene, Settings *settings);
    QImage generate( QImage image) override;


private:
    void drawEdges(QPainter &painter, const std::vector<Vector2D> points);
};

#endif // WIREFRAMER_H
