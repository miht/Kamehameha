#include "wireframer.h"

Wireframer::Wireframer(QWidget *parent, Scene *scene, Settings *settings)
    :   Renderer(parent, scene, settings)
{
    mode = Renderer::Wireframer;
}

QImage Wireframer::generate(QImage image) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";

    QPainter painter (&image);
    painter.fillRect (0,0,image.width (), image.height (), Qt::black);
    painter.setPen (Qt::white);


    // Iterate over all pixels in image
    for(Face* face : scene->model.root->faces) {
        std::vector<Vector2D> points;

        int vertices = face->getPoints ().size();
        for(size_t i = 0; i < vertices; i++) {
            points.push_back (worldToViewport (face->getPoints ()[i]));
        }

        drawEdges(painter, points);
    }

    painter.end ();
    return image;
}

void Wireframer::drawEdges(QPainter &painter, const std::vector<Vector2D> points) {
    int n = points.size();
    for(int i = 0; i < n; i++) {
        Vector2D p1 = points[i];
        Vector2D p2 = points[(i + 1) % n];

        //  TODO CHECK IF OUTSIDE OF SCREEN BOUNDS
        painter.drawLine(p1.x, p1.y, p2.x, p2.y);
    }
}

