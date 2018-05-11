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
    QImage generate(QProgressBar *progress, QImage image) override;

private:
    void computePolygonRows(const std::vector<Vector2D>& vertexPixels, std::vector<Vector2D>& leftPixels, std::vector<Vector2D>& rightPixels);
    void drawPolygon(QPainter &painter, const std::vector<Vector2D> &leftPixels, const std::vector<Vector2D> &rightPixels);
    void drawRows(QPainter &painter, const std::vector<Vector2D>& leftPixels, const std::vector<Vector2D>& rightPixels);

};

#endif // RASTERIZER_H
