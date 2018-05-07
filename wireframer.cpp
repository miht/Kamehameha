#include "wireframer.h"

Wireframer::Wireframer(Scene *scene)
    :   Renderer(scene)
{
    mode = Renderer::Wireframer;
}

QImage Wireframer::generate(QProgressBar *progress, QImage image) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    QPainter painter (&image);
    painter.fillRect (0,0,image.width (), image.height (), Qt::black);
    painter.setPen (Qt::white);

    float width = (float) image.width ();
    float height = (float) image.height ();

    float canvasWidth = width/height, canvasHeight = 1;

    Matrix4x4 worldToCam;
    bool inverted = Matrix4x4::inverse(scene->camera.camToWorld, worldToCam);
    // Iterate over all pixels in image
    for(Face* face : scene->model.root->faces) {
        std::vector<Vector2D> points;

        int vertices = face->getPoints ().size();
        for(size_t i = 0; i < vertices; i++) {
            Vector3D pWorld = face->getPoints()[i];
            Vector3D pCamera = worldToCam*pWorld;
            Vector2D pScreen(pCamera.x / -pCamera.z, pCamera.y / -pCamera.z);

//            if (std::abs(pScreen.x) <= canvasWidth || std::abs(pScreen.y) <= canvasHeight) {
                Vector2D pNDC((pScreen.x + canvasWidth * 0.5) / canvasWidth, (pScreen.y + canvasHeight * 0.5) / canvasHeight);

                int pX = (int)(pNDC.x * width);
                int pY = (int)((1 - pNDC.y) * height);

                points.push_back (Vector2D(pX, pY));
//            }
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

