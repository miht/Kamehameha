#include "wireframer.h"

Wireframer::Wireframer(Scene &scene)
    :   Renderer(scene)
{
}

Wireframer::Wireframer(int w, int h, Scene &scene)
    :   Renderer(w, h, scene)
{

}

QImage Wireframer::generate(QProgressBar *progress, int xOffset, int yOffset, int width, int height) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    QImage img = QImage(width, height, QImage::Format_RGB32);
    QPainter painter (&img);
    painter.fillRect (0,0,width,height, Qt::white);
    painter.setPen (Qt::black);

    // Iterate over all pixels in image
    for(Shape* shape : scene.model.shapes) {
        QPointF* qPoints = new QPointF[shape->getPoints ().size ()];
        for(size_t i = 0; i < shape->getPoints ().size (); i++) {
            Vector3D screenCoords = worldToScreenCoordinates (shape->getPoints ()[i]);
            qPoints[i] =  QPointF(screenCoords.x, screenCoords.y);
        }

        if(shape->type == Shape::triangle) {
            //remember to convert QPoints to screen coordinates!
            painter.drawPolygon (qPoints, 3, Qt::OddEvenFill);
        }
    }
    painter.end ();
    return img;
}
