#include "rasterizer.h"

Rasterizer::Rasterizer(Scene *scene)
    :   Renderer(scene)
{
}

Rasterizer::Rasterizer(int w, int h, Scene *scene)
    :   Renderer(w, h, scene)
{
}

QImage Rasterizer::generate(QProgressBar *progress, int xOffset, int yOffset, int width, int height) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    QImage img = QImage(width, height, QImage::Format_RGB32);
    QPainter painter (&img);
    painter.fillRect (0,0,width,height, Qt::white);
    painter.setPen (Qt::black);

    // Iterate over all pixels in image, paint them
    for(Object object : scene->model.objects) {
        for(Face* face : object.faces) {
            QPointF* qPoints = new QPointF[face->getPoints ().size ()];
            for(size_t i = 0; i < face->getPoints ().size (); i++) {
                Vector3D screenCoords = worldToScreenCoordinates (face->getPoints ()[i]);
                qPoints[i] =  QPointF(screenCoords.x, screenCoords.y);
            }

            if(face->type == Face::triangle) {
                //remember to convert QPoints to screen coordinates!
                painter.setPen (Qt::black);
                painter.drawPolygon (qPoints, 3, Qt::OddEvenFill);

                painter.setPen (Qt::green);
                //Test drawing the normals of the face
                for(Vertex3D v : face->getVertices ()) {
                    Vector3D sc = worldToScreenCoordinates (v.position);
                    Vector3D sc2 = worldToScreenCoordinates (v.position + 0.05 * v.normal.normalized ());
                    painter.drawLine(sc.x, sc.y, sc2.x, sc2.y);
                }
                //Draw normals of faces
                painter.setPen (Qt::red);
                Vector3D midPoint = 0.33333 * (face->getVertices()[0].position +
                        face->getVertices()[1].position + face->getVertices()[2].position);
                Vector3D midPoint_screen = worldToScreenCoordinates (midPoint);

                Vector3D e1 = face->getVertices ()[1].position - face->getVertices()[0].position;
                Vector3D e2 = face->getVertices ()[2].position - face->getVertices ()[0].position;
                Vector3D midPoint_screen2 = worldToScreenCoordinates (midPoint
                                                                     + 0.05 * Vector3D::cross_prod (e1, e2).normalized ());
                painter.drawLine(midPoint_screen.x, midPoint_screen.y, midPoint_screen2.x, midPoint_screen2.y);

            }

            delete [] qPoints;
        }
    }

    painter.end ();
    return img;
}
